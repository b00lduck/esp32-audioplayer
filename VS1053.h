/**
 * 
 * Copyright 2017 D.Zerlett <daniel@zerlett.eu>
 * 
 * This file is based on the VS1053 library by maniacbug alias J.Coliz (https://github.com/maniacbug/VS1053)
 * 
 * This file is part of esp8266-audioplayer.
 * 
 * esp8266-audioplayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * esp8266-audioplayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with esp8266-audioplayer. If not, see <http://www.gnu.org/licenses/>.
 *  
 */
#include <Arduino.h>
#include <SPI.h>
#include "csmux.h"

class VS1053 {
  private:
    CSMultiplexer *csMux;
    uint8_t       xcs_address;
    uint8_t       xdcs_address;
    uint8_t       xreset_address;
    uint8_t       dreq_pin;    
    
    uint8_t       curvol;
    const uint8_t vs1053_chunk_size = 32 ;
    
    // SCI Register
    const uint8_t SCI_MODE          = 0x0 ;
    const uint8_t SCI_BASS          = 0x2 ;
    const uint8_t SCI_CLOCKF        = 0x3 ;
    const uint8_t SCI_AUDATA        = 0x5 ;
    const uint8_t SCI_WRAM          = 0x6 ;
    const uint8_t SCI_WRAMADDR      = 0x7 ;
    const uint8_t SCI_AIADDR        = 0xA ;
    const uint8_t SCI_VOL           = 0xB ;
    const uint8_t SCI_AICTRL0       = 0xC ;
    const uint8_t SCI_AICTRL1       = 0xD ;
    const uint8_t SCI_num_registers = 0xF ;
    
    // SCI_MODE bits
    const uint8_t SM_SDINEW         = 11 ;            // Bitnumber in SCI_MODE always on
    const uint8_t SM_RESET          = 2 ;             // Bitnumber in SCI_MODE soft reset
    const uint8_t SM_CANCEL         = 3 ;             // Bitnumber in SCI_MODE cancel song
    const uint8_t SM_TESTS          = 5 ;             // Bitnumber in SCI_MODE for tests
    const uint8_t SM_LINE1          = 14 ;            // Bitnumber in SCI_MODE for Line input
    
    SPISettings   VS1053_SPI ;                        // SPI settings for this slave
    uint8_t       endFillByte ;                       // Byte to send when stopping song

    __attribute__((aligned(4))) uint8_t chunkbuf[32]; // Chunk buffer
    uint8_t chunkbufcnt = 0;                          // Data in chunk buffer
    
  protected:
    inline void await_data_request() const {
      while (!digitalRead(dreq_pin)) {
        yield() ;                                 // Very short delay
      }
    }

    inline void control_mode_on() const {
      SPI.beginTransaction ( VS1053_SPI ) ;       // Prevent other SPI users
      csMux->chipSelect(xcs_address);
    }

    inline void control_mode_off() const {
      csMux->chipDeselect();
      SPI.endTransaction() ;                      // Allow other SPI users
    }

    inline void data_mode_on() const {
      SPI.beginTransaction ( VS1053_SPI ) ;       // Prevent other SPI users
      csMux->chipSelect(xdcs_address);
    }

    inline void data_mode_off() const {
      csMux->chipDeselect();
      SPI.endTransaction() ;                      // Allow other SPI users
    }

    uint16_t read_register ( uint8_t _reg ) const ;
    void     write_register ( uint8_t _reg, uint16_t _value ) const ;
    void     sdi_send_buffer ( uint8_t* data, size_t len ) ;
    void     sdi_send_fillers ( size_t length ) ;
    void     wram_write ( uint16_t address, uint16_t data ) ;
    uint16_t wram_read ( uint16_t address ) ;

  public:
  
    VS1053(CSMultiplexer *csMux, uint8_t _xcs_address, uint8_t _xdcs_address, uint8_t _dreq_pin, uint8_t _xreset_address);
    
    void     begin() ;                                   // Sets pins correctly and prepares SPI bus.
    void     startSong() ;                               // Prepare to start playing. Call this each time a new song starts.
    void     playChunk ( uint8_t* data, size_t len ) ;   // Play a chunk of data.  Copies the data to
    // the chip.  Blocks until complete.
    void     stopSong() ;                                // Finish playing a song. Call this after
    // the last playChunk call.
    void     setVolume ( uint8_t vol ) ;                 // Set the player volume.Level from 0-100,
    // higher is louder.
    void     setTone ( uint8_t* rtone ) ;                // Set the player baas/treble, 4 nibbles for
    // treble gain/freq and bass gain/freq
    uint8_t  getVolume() ;                               // Get the currenet volume setting.
    // higher is louder.
    void     printDetails () ;       // Print configuration details to serial output.
    void     softReset() ;                               // Do a soft reset
    bool     testComm ( const char *header ) ;           // Test communication with module
    inline bool data_request() const {
      return ( digitalRead ( dreq_pin ) == HIGH ) ;
    }

    void     processByte (uint8_t b, bool force);
    
} ;
