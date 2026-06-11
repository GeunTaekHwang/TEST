#pragma once

#include <string>
#include <vector>
#include "WeatherData.h"

class WeatherApiClient
{
public:
    WeatherApiClient();
    ~WeatherApiClient();

    // Set API key
    void SetApiKey(const std::string& apiKey);

    // Get weather by city name
    ApiResponse GetWeatherByCity(const std::string& city);

    // Get weather by coordinates
    ApiResponse GetWeatherByCoordinates(double latitude, double longitude);

    // Get 5-day forecast
    std::vector<ForecastData> GetForecast(const std::string& city);

private:
    std::string m_apiKey;
    std::string m_apiBaseUrl;
    int m_timeout;

    // HTTP request handling
    std::string MakeHttpRequest(const std::string& url);

    // JSON parsing
    ApiResponse ParseWeatherJson(const std::string& jsonStr);
    std::vector<ForecastData> ParseForecastJson(const std::string& jsonStr);

    // Error handling
    ApiResponse CreateErrorResponse(const std::string& error);
};
