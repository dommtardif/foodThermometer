//Fonts
#define DEF_FONT u8g2_font_7x13_t_symbols
#define SMALL_FONT u8g2_font_profont10_tr
#define SMLINE_HEIGHT 8
#define LINE_HEIGHT 11

//Cooking stage description
typedef struct {
  char description [25];
  int temp;
} descriptionType;

//Operation mode
#define DEFAULT_MODE 1
#define CANDY_MODE 2
#define BEEF_MODE 3
#define PORC_MODE 4
#define POULTRY_MODE 5
#define ALARM_MODE 6
#define CALIB_MODE 7

//Exponential moving average smoothing for delta
#define SMOOTHING_FACTOR 0.1

const static descriptionType PROGMEM candyStages[11]  = {{"Confiserie-Candy making", -5000},
  {"Petit filet/Soft thread", 10500},
  {"Grand filet/Thread", 10700},
  {"Petit boule/Soft ball", 11200},
  {"Grand boule/Firm ball", 11800},
  {"Boule/Hard ball", 12500},
  {"Petit casse/Soft crack", 13500},
  {"Grand casse/Hard crack", 14500},
  {"Caramel", 15000},
  {"Sucre brule/Burnt sugar", 17100},
  {"", 30000}
};

const static descriptionType PROGMEM beefStages[5]  = {{"Boeuf/Agneau-Beef/Lamb", -5000},
  {"Saignant/Rare", 6000},
  {"A point/Medium", 7000},
  {"Bien cuit/Well done", 7500},
  {"", 30000}
};

const static descriptionType PROGMEM porcStages[4]  = {{"Porc", -5000},
  {"Fume/Smoked", 6000},
  {"Frais/Fresh", 7500},
  {"", 30000}
};

const static descriptionType PROGMEM poultryStages[5]  = {{"Volaille-Poultry", -5000},
  {"Poulet/Chicken min", 8000},
  {"Poulet/Chicken max", 8500},
  {"Canard/Duck", 9000},
  {"", 30000}
};
