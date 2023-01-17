#ifndef structs_h
#define structs_h
struct Prices
{
    float hour_prices[24];
    float min;
    float max;
    float average;
    float peak;
    float off_peak_1;
    float off_peak_2;

    void print_prices()
    {
        // for (float& p : hour_prices)
        for (int i = 0; i < sizeof(hour_prices); i++)
        {
            Serial.print("time of day: ");
            Serial.print(i);
            Serial.print(", value: ");
            Serial.println(p);
        }

        Serial.print(", meta_min: ");
        Serial.println(prices->min);
        Serial.print(", meta_max: ");
        Serial.println(prices->max);
        Serial.print(", meta_average: ");
        Serial.println(prices->average);
        Serial.print(", meta_peak: ");
        Serial.println(prices->peak);
        Serial.print(", meta_off_peak_1: ");
        Serial.println(prices->off_peak_1);
        Serial.print(", meta_off_peak_2: ");
        Serial.println(prices->off_peak_2);
    }
};

#endif