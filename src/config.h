
// Digital pins for LEDs
#define LED1                    26
#define LED2                    27
//#define LED3                    14
//#define LED4                    12

// Digital pins for buttons
#define BUTTON1                 36
#define BUTTON2                 39
#define BUTTON3                 34
#define HEADPHONE_SWITCH        35

// Amplifier enable/disable (power saving)
#define AMP_ENABLE              25

// Battery monitoring
#define ADC_BATT                 32
#define LOW_BATT                 33

// Digital pins for VS1053 
#define VS1053_XCS_PIN          15
#define VS1053_XDCS_PIN         16
#define VS1053_DREQ_PIN         17
#define VS1053_XRESET_PIN       2 

// Digital pins for SD card
#define SD_CS_PIN               5

// Digital pins for MFRC522
#define MFRC522_CS_PIN          4
#define MFRC522_RST_PIN         13

#define SPI_SCK_PIN             18
#define SPI_MISO_PIN            19
#define SPI_MOSI_PIN            23

// Digital pins for I²C 
#define I2C_SDA_PIN             21
#define I2C_SCL_PIN             22

// Display I²C address
#define DISPLAY_ADDRESS         0x3c

// Number of bytes for the card ID
#define ID_BYTE_ARRAY_LENGTH    4

#define OLED
#define TWI_CLOCK 600000UL


//#define FAIL_ON_FILE_NOT_FOUND
#define FAST_BOOT
#define MAX_PLAYLIST_LENGTH 32
#define MAX_FILENAME_LENGTH 64