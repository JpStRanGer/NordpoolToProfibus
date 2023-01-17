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
        for (size_t i = 0; i < sizeof(hour_prices); i++)
        {
            Serial.print("time of day: ");
            Serial.print(i);
            Serial.print(", value: ");
            Serial.println(hour_prices[i]);
        }

        Serial.print(", meta_min: ");
        Serial.println(min);
        Serial.print(", meta_max: ");
        Serial.println(max);
        Serial.print(", meta_average: ");
        Serial.println(average);
        Serial.print(", meta_peak: ");
        Serial.println(peak);
        Serial.print(", meta_off_peak_1: ");
        Serial.println(off_peak_1);
        Serial.print(", meta_off_peak_2: ");
        Serial.println(off_peak_2);
    }

};

#endif