#pragma once

#include <string>
#include <vector>

// Weather data structure
struct WeatherData
{
    std::string city;
    std::string country;
    double temperature;
    double feels_like;
    double temp_min;
    double temp_max;
    int humidity;
    int pressure;
    double wind_speed;
    std::string description;
    std::string main;
    int clouds;
    long sunrise;
    long sunset;
    int visibility;
    double uvi;
};

// 5-day forecast structure
struct ForecastData
{
    long dt;
    double temp_day;
    double temp_night;
    double temp_max;
    double temp_min;
    int humidity;
    int pressure;
    std::string description;
    std::string main;
    double rain_probability;
    double wind_speed;
    int clouds;
};

// API response structure
struct ApiResponse
{
    bool success;
    std::string error_message;
    WeatherData weather;
    std::vector<ForecastData> forecast;
};
