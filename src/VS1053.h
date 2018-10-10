/**
 * 
 * Copyright 2018 D.Zerlett <daniel@zerlett.eu>
 * 
 * This file is based on the VS1053 library by maniacbug alias J.Coliz (https://github.com/maniacbug/VS1053)
 * 
 * This file is part of esp32-audioplayer.
 * 
 * esp32-audioplayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * esp32-audioplayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with esp32-audioplayer. If not, see <http://www.gnu.org/licenses/>.
 *  
 */
#include <Arduino.h>
#include <SPI.h>

class VS1053 {
  private:
    uint8_t       xcsPin;
    uint8_t       xdcsPin;
    uint8_t       xresetPin;
    uint8_t       dreqPin;    
    
    uint8_t       curvol;
    const uint8_t vs1053ChunkSize = 32 ;
    
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
      while (!digitalRead(dreqPin)) {
        yield() ;                                 // Very short delay
      }
    }

    inline void controlModeOn() const {
      SPI.beginTransaction ( VS1053_SPI ) ;       // Prevent other SPI users
      digitalWrite(xcsPin, LOW);
    }

    inline void controlModeOff() const {
      digitalWrite(xcsPin, HIGH);
      SPI.endTransaction() ;                      // Allow other SPI users
    }

    inline void dataModeOn() const {
      SPI.beginTransaction ( VS1053_SPI ) ;       // Prevent other SPI users
      digitalWrite(xdcsPin, LOW);
    }

    inline void dataModeOff() const {
      digitalWrite(xdcsPin, HIGH);
      SPI.endTransaction() ;                      // Allow other SPI users
    }

    uint16_t read_register ( uint8_t _reg ) const ;
    void     write_register ( uint8_t _reg, uint16_t _value ) const ;
    void     sdi_send_buffer ( uint8_t* data, size_t len ) ;
    void     sdi_send_fillers ( size_t length ) ;
    void     wram_write ( uint16_t address, uint16_t data ) ;
    uint16_t wram_read ( uint16_t address ) ;

  public:
  
    VS1053(uint8_t _xcsPin, uint8_t _xdcsPin, uint8_t _dreqPin, uint8_t _xresetPin);
    
    void     begin();                                     // Sets pins correctly and prepares SPI bus.
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
      return ( digitalRead ( dreqPin ) == HIGH ) ;
    }

    void     processByte (uint8_t b, bool force);
    
} ;
