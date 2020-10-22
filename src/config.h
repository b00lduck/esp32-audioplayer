
// Buttons
#define BUTTON0_PIN         22 // vol -
#define BUTTON1_PIN         39 // left
#define BUTTON2_PIN         36 // on/play/pause
#define BUTTON3_PIN         35 // right
#define BUTTON4_PIN         21 // vol +

// Amplifier power enable
#define AMP_ENABLE_PIN      33

// Power down
#define SHUTDOWN_PIN        27

// WS2812 data line
#define WS2812_DATA_PIN     26

// Battery monitoring
#define ADC_BATT            32
#define ADC_DIVISOR         550.

// MP3 decode (VS1053)
#define VS1053_XCS_PIN      15
#define VS1053_XDCS_PIN     16
#define VS1053_DREQ_PIN     17
#define VS1053_XRESET_PIN   2 

// Common SPI config (RFID and MMC/SD)
#define SPI_SCK_PIN         18
#define SPI_MISO_PIN        19
#define SPI_MOSI_PIN        23

// MMC/SD card
#define SD_CS_PIN           5

// RFID (MFRC522)
#define MFRC522_CS_PIN      4
#define MFRC522_RST_PIN     13


// Number of bytes for the card ID
#define ID_BYTE_ARRAY_LENGTH    4

//#define FAIL_ON_FILE_NOT_FOUND
//#define FAST_BOOT
#define MAX_PLAYLIST_LENGTH 32
#define MAX_FILENAME_LENGTH 64