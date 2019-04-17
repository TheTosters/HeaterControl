#pragma once

extern "C" {
  #include "boards.h"
  #include "nrf_gpio.h"
  #include "nrf_delay.h"
}

#include "types/hardware_pin.h"

#include <stdint.h>
#include <algorithm>
#include <array>

//based on https://github.com/PaulStoffregen/OneWire/blob/master/OneWire.h
class OneWire {
public:
  using DeviceAddress = std::array<uint8_t, 8>;

  explicit OneWire(HardwarePin pinNo) : wirePin(pinNo) {
  }

  void resetSearch() {
    lastDiscrepancy = 0;
    lastDeviceFlag = false;
    lastFamilyDiscrepancy = 0;
    std::fill(std::begin(discoveredAddress), std::end(discoveredAddress), 0);
  }

  DeviceAddress search(bool search_mode) {
    uint8_t id_bit_number;
    uint8_t last_zero, rom_byte_number;
    bool search_result;
    uint8_t id_bit, cmp_id_bit;

    uint8_t rom_byte_mask, search_direction;

    // initialize for search
    id_bit_number = 1;
    last_zero = 0;
    rom_byte_number = 0;
    rom_byte_mask = 1;
    search_result = false;

    // if the last call was not the last one
    if (not lastDeviceFlag) {
      // 1-Wire reset
      if (not reset()) {
        // reset the search
        lastDiscrepancy = 0;
        lastDeviceFlag = false;
        lastFamilyDiscrepancy = 0;
        return {};
      }

      // issue the search command
      if (search_mode == true) {
        write(0xF0);   // NORMAL SEARCH
      } else {
        write(0xEC);   // CONDITIONAL SEARCH
      }

      // loop to do the search
      do {
        // read a bit and its complement
        id_bit = read_bit();
        cmp_id_bit = read_bit();

        // check for no devices on 1-wire
        if ((id_bit == 1) && (cmp_id_bit == 1)) {
          break;
        } else {
          // all devices coupled have 0 or 1
          if (id_bit != cmp_id_bit) {
            search_direction = id_bit;  // bit write value for search
          } else {
            // if this discrepancy if before the Last Discrepancy
            // on a previous next then pick the same as last time
            if (id_bit_number < lastDiscrepancy) {
              search_direction =
                  ((discoveredAddress[rom_byte_number] & rom_byte_mask) > 0);
            } else {
              // if equal to last pick 1, if not then pick 0
              search_direction = (id_bit_number == lastDiscrepancy);
            }
            // if 0 was picked then record its position in LastZero
            if (search_direction == 0) {
              last_zero = id_bit_number;

              // check for Last discrepancy in family
              if (last_zero < 9)
                lastFamilyDiscrepancy = last_zero;
            }
          }

          // set or clear the bit in the ROM byte rom_byte_number
          // with mask rom_byte_mask
          if (search_direction == 1)
            discoveredAddress[rom_byte_number] |= rom_byte_mask;
          else
            discoveredAddress[rom_byte_number] &= ~rom_byte_mask;

          // serial number search direction write bit
          write_bit(search_direction);

          // increment the byte counter id_bit_number
          // and shift the mask rom_byte_mask
          id_bit_number++;
          rom_byte_mask <<= 1;

          // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
          if (rom_byte_mask == 0) {
            rom_byte_number++;
            rom_byte_mask = 1;
          }
        }
      } while (rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if (not(id_bit_number < 65)) {
        // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
        lastDiscrepancy = last_zero;

        // check for last device
        if (lastDiscrepancy == 0) {
          lastDeviceFlag = true;
        }
        search_result = true;
      }
    }

    // if no device found then reset counters so next 'search' will be like a first
    if (not search_result || not discoveredAddress[0]) {
      lastDiscrepancy = 0;
      lastDeviceFlag = false;
      lastFamilyDiscrepancy = 0;
      search_result = false;
    } else {
      return discoveredAddress;
    }
    return {};
  }

  uint8_t crc8(uint8_t* data, size_t len) {
    //there is also version with lookup table, refer to
    //https://github.com/PaulStoffregen/OneWire/blob/master/OneWire.cpp
    uint8_t crc = 0;

    while (len--) {
      uint8_t inbyte = *data++;
      for (unsigned int i = 0; i < 8; i++) {
        uint8_t mix = (crc ^ inbyte) & 0x01;
        crc >>= 1;
        if (mix) {
          crc ^= 0x8C;
        }
        inbyte >>= 1;
      }
    }
    return crc;
  }

  bool reset() {
    nrf_gpio_cfg_output(wirePin.get());
    nrf_gpio_pin_clear(wirePin.get());

    nrf_delay_us(500);
    nrf_gpio_pin_set(wirePin.get());

    nrf_gpio_cfg_input(wirePin.get(), NRF_GPIO_PIN_NOPULL);
    nrf_delay_us(70);

    //NOTE: == 0 not != 0
    bool presence = nrf_gpio_pin_read(wirePin.get()) == 0;
    nrf_delay_us(410);

    //NOTE: == 1
    presence = nrf_gpio_pin_read(wirePin.get()) == 1;
    return presence;
  }

  void write(uint8_t data, bool power = false) {
    for (uint8_t bitMask = 0x01; bitMask; bitMask <<= 1) {
      write_bit( (bitMask & data) != 0 );
    }
    nrf_delay_us(100);

    if (not power) {
      nrf_gpio_cfg_input(wirePin.get(), NRF_GPIO_PIN_NOPULL);
      nrf_gpio_pin_clear(wirePin.get());
    }
  }

  void write_bytes(const uint8_t *buf, size_t count, bool power) {
    for (size_t i = 0 ; i < count ; i++) {
      write(buf[i]);
    }
    if (not power) {
      nrf_gpio_cfg_input(wirePin.get(), NRF_GPIO_PIN_NOPULL);
      nrf_gpio_pin_clear(wirePin.get());
    }
  }

  void read_bytes(uint8_t *buf, size_t count) {
    for (size_t i = 0 ; i < count ; i++) {
      buf[i] = read();
    }
  }

  void select(const DeviceAddress& address) {
    write(0x55);
    for(auto& c: address) {
      write(c);
    }
  }

  void skip() {
    write(0xCC);
  }

  bool read_bit() {
    nrf_gpio_cfg_output(wirePin.get());
    nrf_gpio_pin_clear(wirePin.get());

    nrf_delay_us(3);

    nrf_gpio_cfg_input(wirePin.get(), NRF_GPIO_PIN_NOPULL);
    nrf_delay_us(10);

    bool result = nrf_gpio_pin_read(wirePin.get()) != 0;

    nrf_gpio_pin_set(wirePin.get());
    nrf_delay_us(53);

    return result;
  }

  void write_bit(bool high) {
    nrf_gpio_cfg_output(wirePin.get());
    nrf_gpio_pin_clear(wirePin.get());

    if(high) {
      nrf_delay_us(10);
      nrf_gpio_pin_set(wirePin.get());
      nrf_delay_us(55);

    } else {
      nrf_delay_us(65);
      nrf_gpio_pin_set(wirePin.get());
      nrf_delay_us(5);
    }
  }

  uint8_t read() {
    uint8_t r = 0;

    uint16_t bitMask = 0x01;
    for(int t = 0; t < 8; t++) {
      if ( OneWire::read_bit()) {
        r |= static_cast<uint8_t>(bitMask);
      }
      bitMask <<= 1;
    }
    return r;
  }

private:
  HardwarePin wirePin;
  DeviceAddress discoveredAddress;
  uint8_t lastDiscrepancy {};
  uint8_t lastFamilyDiscrepancy {};
  bool lastDeviceFlag {};
};
