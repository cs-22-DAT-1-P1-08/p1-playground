#ifndef TJEK_API_PRIVATE_H
#define TJEK_API_PRIVATE_H

#include <time.h>

#define TJEK_API_BASE_URL "https://squid-api.tjek.com/v2/"
#define TJEK_API_V4_BASE_URL "https://squid-api.tjek.com/v4/"

/**
 * Converts ISO8601 string to time_t object, assuming time string is UTC +0.
 */
time_t parse_iso8601_string(const char *time_str);

#endif