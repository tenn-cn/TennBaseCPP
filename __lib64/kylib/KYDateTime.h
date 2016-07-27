// =======================================
// Unit   : 日期时间类型 (KYDateTime.h)
// Version: 3.0.1.0 (build 2012.06.29)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYDateTime_H_
#define _KYDateTime_H_

#include <time.h>
#include <windows.h>
#include "KYObject.h"
#include "KYString.h"

// KYLib 2.0 开始使用 KYLib 命名空间
namespace KYLib
{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 常量/类型 */

// Seconds and milliseconds per day
const long  SecsPerDay     = 24 * 60 * 60;
const long  MSecsPerDay    = SecsPerDay * 1000;

// Days between 1/1/0001 and 12/31/1899
const long  DateDelta      = 693594;

// 1970.01.01 的 TDateTime 值, = EncodeDate(1970, 1, 1)
const double DT_1970_1_1   = 25569.0;


// 月天数表类型
typedef struct
{
   Word     Days[12];
} TDayTable, *PDayTable;

// 月天数表
const TDayTable Month_Days[2]
      = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
         {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

// 月总天数表
const TDayTable Month_DaySums[2]
      = {{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
         {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}};

/* Date and time record */

typedef struct
{
   long     Time;       // Number of milliseconds since midnight
   long     Date;       // One plus number of days since 1/1/0001
} TTimeStamp, *PTimeStamp;

/* System Locale information record */

typedef struct
{
   LCID     DefaultLCID;
   LANGID   PriLangID;
   LANGID   SubLangID;
   bool     FarEast;
   bool     MiddleEast;
} TSysLocale, *PSysLocale;

/* 说明: TDateTime 的整数部分为日期, 小数部分为时间. 日期值为与 1899-12-30 差值,
   时间值为从0点开始的毫秒数在一天毫秒数的比例值. */

typedef double TDateTime;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 格式变量 */

/* date/time formatting options

  The initial values of these variables are fetched from the system registry
  using the GetLocaleInfo function in the Win32 API. The description of each
  variable specifies the LOCALE_XXXX constant used to fetch the initial
  value.

  DateSeparator - The character used to separate the year, month, and day
  parts of a date value. The initial value is fetched from LOCATE_SDATE.

  ShortDateFormat - The format string used to convert a date value to a
  short string suitable for editing. For a complete description of date and
  time format strings, refer to the documentation for the FormatDate
  function. The short date format should only use the date separator
  character and the  m, mm, d, dd, yy, and yyyy format specifiers. The
  initial value is fetched from LOCALE_SSHORTDATE.

  LongDateFormat - The format string used to convert a date value to a long
  string suitable for display but not for editing. For a complete description
  of date and time format strings, refer to the documentation for the
  FormatDate function. The initial value is fetched from LOCALE_SLONGDATE.

  TimeSeparator - The character used to separate the hour, minute, and
  second parts of a time value. The initial value is fetched from
  LOCALE_STIME.

  TimeAMString - The suffix string used for time values between 00:00 and
  11:59 in 12-hour clock format. The initial value is fetched from
  LOCALE_S1159.

  TimePMString - The suffix string used for time values between 12:00 and
  23:59 in 12-hour clock format. The initial value is fetched from
  LOCALE_S2359.

  ShortTimeFormat - The format string used to convert a time value to a
  short string with only hours and minutes. The default value is computed
  from LOCALE_ITIME and LOCALE_ITLZERO.

  LongTimeFormat - The format string used to convert a time value to a long
  string with hours, minutes, and seconds. The default value is computed
  from LOCALE_ITIME and LOCALE_ITLZERO.

  ShortMonthNames - Array of strings containing short month names. The mmm
  format specifier in a format string passed to FormatDate causes a short
  month name to be substituted. The default values are fecthed from the
  LOCALE_SABBREVMONTHNAME system locale entries.

  LongMonthNames - Array of strings containing long month names. The mmmm
  format specifier in a format string passed to FormatDate causes a long
  month name to be substituted. The default values are fecthed from the
  LOCALE_SMONTHNAME system locale entries.

  ShortDayNames - Array of strings containing short day names. The ddd
  format specifier in a format string passed to FormatDate causes a short
  day name to be substituted. The default values are fecthed from the
  LOCALE_SABBREVDAYNAME system locale entries.

  LongDayNames - Array of strings containing long day names. The dddd
  format specifier in a format string passed to FormatDate causes a long
  day name to be substituted. The default values are fecthed from the
  LOCALE_SDAYNAME system locale entries.

  TwoDigitYearCenturyWindow - Determines what century is added to two
  digit years when converting string dates to numeric dates.  This value
  is subtracted from the current year before extracting the century.
  This can be used to extend the lifetime of existing applications that
  are inextricably tied to 2 digit year data entry.  The best solution
  to Year 2000 (Y2k) issues is not to accept 2 digit years at all - require
  4 digit years in data entry to eliminate century ambiguities.

  Examples:

  Current TwoDigitCenturyWindow  Century  StrToDate() of:
  Year    Value                  Pivot    '01/01/03' '01/01/68' '01/01/50'
  -------------------------------------------------------------------------
  1998    0                      1900     1903       1968       1950
  2002    0                      2000     2003       2068       2050
  1998    50 (default)           1948     2003       1968       1950
  2002    50 (default)           1952     2003       1968       2050
  2020    50 (default)           1970     2003       2068       2050
 */

extern TSysLocale SysLocale;

extern char       DateSeparator;
extern char       TimeSeparator;

extern KYString   ShortDateFormat;
extern KYString   LongDateFormat;
extern KYString   TimeAMString;
extern KYString   TimePMString;
extern KYString   ShortTimeFormat;
extern KYString   LongTimeFormat;
extern KYString   ShortMonthNames[12];
extern KYString   LongMonthNames[12];
extern KYString   ShortDayNames[7];
extern KYString   LongDayNames[7];
extern Word       TwoDigitYearCenturyWindow;

extern KYString   EraNames[7];
extern long       EraYearOffsets[7];

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 初始化函数 */

// 初始化格式设置
void              InitFormatSettings();

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 常用函数 */

// 返回除数和余数
inline long DivMod(long ADividend, long ADivisor, long& ARemainder)
{
   ldiv_t Value;

   Value       = ldiv(ADividend, ADivisor);
   ARemainder  = Value.rem;

   return Value.quot;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TDateTime 相关函数 */

TTimeStamp  DateTimeToTimeStamp(TDateTime ADateTime);
TDateTime   TimeStampToDateTime(const TTimeStamp& ATimeStamp);

/* EncodeDate encodes the given year, month, and day into a TDateTime value.
   The year must be between 1 and 9999, the month must be between 1 and 12,
   and the day must be between 1 and N, where N is the number of days in the
   specified month. If the specified values are not within range, an
   EConvertError exception is raised. The resulting value is the number of
   days between 12/30/1899 and the given date. */

TDateTime EncodeDate(Word Year, Word Month, Word Day);

/* EncodeTime encodes the given hour, minute, second, and millisecond into a
   TDateTime value. The hour must be between 0 and 23, the minute must be
   between 0 and 59, the second must be between 0 and 59, and the millisecond
   must be between 0 and 999. If the specified values are not within range, an
   EConvertError exception is raised. The resulting value is a number between
   0 (inclusive) and 1 (not inclusive) that indicates the fractional part of
   a day given by the specified time. The value 0 corresponds to midnight,
   0.5 corresponds to noon, 0.75 corresponds to 6:00 pm, etc. */

TDateTime EncodeTime(Word Hour, Word Min, Word Sec, Word MSec);

/* DecodeDate decodes the integral (date) part of the given TDateTime value
   into its corresponding year, month, and day. If the given TDateTime value
   is less than or equal to zero, the year, month, and day return parameters
   are all set to zero. */

void DecodeDate(TDateTime ADate, Word& Year, Word& Month, Word& Day);

/* This variation of DecodeDate works similarly to the above function but
   returns more information.  The result value of this function indicates
   whether the year decoded is a leap year or not.  */

bool DecodeDateFully(TDateTime ADate, Word& Year, Word& Month, Word& Day, Word& Week);

/* DecodeTime decodes the fractional (time) part of the given TDateTime value
   into its corresponding hour, minute, second, and millisecond. */

void DecodeTime(TDateTime ATime, Word& Hour, Word& Min, Word& Sec, Word& MSec);

/* DateTimeToSystemTime converts a date and time from TDateTime
   format into the Win32 API's SYSTEMTIME format. */

void DateTimeToSystemTime(TDateTime ADateTime, SYSTEMTIME& ASystemTime);

/* SystemTimeToDateTime converts a date and time from the Win32 API's
   SYSTEMTIME format into TDateTime format. */

TDateTime SystemTimeToDateTime(const SYSTEMTIME& ASystemTime);

/* DateTime2GMT converts a date and time from local time
   into Greenwich Mean Time. */

TDateTime LocalTime2GMT(TDateTime ADateTime);

/* GMT2LocalTime converts a date and time from Greenwich Mean Time
   into local time. */

TDateTime GMT2LocalTime(TDateTime ADateTime);

/* DateTime2tm converts a date and time from TDateTime
   format into the Microsoft C++ tm structure format. */

void DateTime2tm(TDateTime ADateTime, tm& ATMTime);

/* tm2DateTime converts a date and time from the Microsoft C++
   tm structure format into TDateTime format. */

TDateTime tm2DateTime(const tm& ATMTime);

/* DateTime2time_t converts a date and time from TDateTime
   format into the Microsoft C++ time_t format. */

time_t DateTime2time_t(TDateTime ADateTime);

/* time_t2DateTime converts a date and time from the Microsoft C++
   time_t format into TDateTime format. */

TDateTime time_t2DateTime(time_t ATime);

/* DayOfWeek returns the day of the week of the given date. The result is an
   integer between 1 and 7, corresponding to Sunday through Saturday. */

inline long DayOfWeek(TDateTime ADate)
{
   long result = ((long)ADate + DateDelta) % 7;
   return (result >= 0) ? result + 1 : result + 8;
}

/* Date returns the current date. */

TDateTime Date();

/* Time returns the current time. */

TDateTime Time();

/* Now returns the current date and time, corresponding to Date + Time. */

TDateTime Now();

/* GMTime returns the current Greenwich Mean Time. */

TDateTime GMTime();

/* Current year returns the year portion of the date returned by Now */

Word CurrentYear();

/* IncMonth returns Date shifted by the specified number of months.
   NumberOfMonths parameter can be negative, to return a date N months ago.
   If the input day of month is greater than the last day of the resulting
   month, the day is set to the last day of the resulting month.
   Input time of day is copied to the DateTime result. */

TDateTime IncMonth(TDateTime ADate, long ANumberOfMonths);

/* ReplaceTime replaces the time portion of the DateTime parameter with the given
   time value, adjusting the signs as needed if the date is prior to 1900
   (Date value less than zero) */

void ReplaceTime(TDateTime& ADateTime, TDateTime ANewTime);

/* ReplaceDate replaces the date portion of the DateTime parameter with the given
   date value, adjusting as needed for negative dates */

void ReplaceDate(TDateTime& ADateTime, TDateTime ANewDate);

/* IsLeapYear determines whether the given year is a leap year. */

inline bool IsLeapYear(Word Year)
{
   return ((Year & 3) == 0) && (((Year % 100) != 0) || ((Year % 400) == 0));
}

/* The MonthDays array can be used to quickly find the number of
   days in a month:  MonthDays(IsLeapYear(Y), M) */

inline Word MonthDays(bool AIsLeapYear, Word Month)
{
   return Month_Days[AIsLeapYear].Days[Month - 1];
}

/* DateToStr converts the date part of the given TDateTime value to a string.
   The conversion uses the format specified by the ShortDateFormat global
   variable. */

KYString DateToStr(TDateTime ADate);

/* TimeToStr converts the time part of the given TDateTime value to a string.
   The conversion uses the format specified by the LongTimeFormat global
   variable. */

KYString TimeToStr(TDateTime ATime);

/* DateTimeToStr converts the given date and time to a string. The resulting
   string consists of a date and time formatted using the ShortDateFormat and
   LongTimeFormat global variables. Time information is included in the
   resulting string only if the fractional part of the given date and time
   value is non-zero. */

KYString DateTimeToStr(TDateTime ADateTime);

/* StrToDate converts the given string to a date value. The string must
   consist of two or three numbers, separated by the character defined by
   the DateSeparator global variable. The order for month, day, and year is
   determined by the ShortDateFormat global variable--possible combinations
   are m/d/y, d/m/y, and y/m/d. If the string contains only two numbers, it
   is interpreted as a date (m/d or d/m) in the current year. Year values
   between 0 and 99 are assumed to be in the current century. If the given
   string does not contain a valid date, an EConvertError exception is
   raised. */

TDateTime StrToDate(const KYString& AStr);
bool      StrToDate(const KYString& AStr, TDateTime& AValue);
TDateTime StrToDateDef(const KYString& AStr, TDateTime ADefault);

/* StrToTime converts the given string to a time value. The string must
   consist of two or three numbers, separated by the character defined by
   the TimeSeparator global variable, optionally followed by an AM or PM
   indicator. The numbers represent hour, minute, and (optionally) second,
   in that order. If the time is followed by AM or PM, it is assumed to be
   in 12-hour clock format. If no AM or PM indicator is included, the time
   is assumed to be in 24-hour clock format. If the given string does not
   contain a valid time, an EConvertError exception is raised. */

TDateTime StrToTime(const KYString& AStr);
bool      StrToTime(const KYString& AStr, TDateTime& AValue);
TDateTime StrToTimeDef(const KYString& AStr, TDateTime ADefault);

/* StrToDateTime converts the given string to a date and time value. The
   string must contain a date optionally followed by a time. The date and
   time parts of the string must follow the formats described for the
   StrToDate and StrToTime functions. */

TDateTime StrToDateTime(const KYString& AStr);
bool      StrToDateTime(const KYString& AStr, TDateTime& AValue);
TDateTime StrToDateTimeDef(const KYString& AStr, TDateTime ADefault);

/* FormatDateTime formats the date-and-time value given by DateTime using the
   format given by Format. The following format specifiers are supported:

  c       Displays the date using the format given by the ShortDateFormat
          global variable, followed by the time using the format given by
          the LongTimeFormat global variable. The time is not displayed if
          the fractional part of the DateTime value is zero.

  d       Displays the day as a number without a leading zero (1-31).

  dd      Displays the day as a number with a leading zero (01-31).

  ddd     Displays the day as an abbreviation (Sun-Sat) using the strings
          given by the ShortDayNames global variable.

  dddd    Displays the day as a full name (Sunday-Saturday) using the strings
          given by the LongDayNames global variable.

  ddddd   Displays the date using the format given by the ShortDateFormat
          global variable.

  dddddd  Displays the date using the format given by the LongDateFormat
          global variable.

  e       Displays the year in the current period/era as a number without
          a leading zero (Japanese, Korean and Taiwanese locales only).

  ee      Displays the year in the current period/era as a number with
          a leading zero (Japanese, Korean and Taiwanese locales only).

  m       Displays the month as a number without a leading zero (1-12). If
          the m specifier immediately follows an h or hh specifier, the
          minute rather than the month is displayed.

  mm      Displays the month as a number with a leading zero (01-12). If
          the mm specifier immediately follows an h or hh specifier, the
          minute rather than the month is displayed.

  mmm     Displays the month as an abbreviation (Jan-Dec) using the strings
          given by the ShortMonthNames global variable.

  mmmm    Displays the month as a full name (January-December) using the
          strings given by the LongMonthNames global variable.

  yy      Displays the year as a two-digit number (00-99).

  yyyy    Displays the year as a four-digit number (0000-9999).

  h       Displays the hour without a leading zero (0-23).

  hh      Displays the hour with a leading zero (00-23).

  n       Displays the minute without a leading zero (0-59).

  nn      Displays the minute with a leading zero (00-59).

  s       Displays the second without a leading zero (0-59).

  ss      Displays the second with a leading zero (00-59).

  z       Displays the millisecond without a leading zero (0-999).

  zzz     Displays the millisecond with a leading zero (000-999).

  t       Displays the time using the format given by the ShortTimeFormat
          global variable.

  tt      Displays the time using the format given by the LongTimeFormat
          global variable.

  am/pm   Uses the 12-hour clock for the preceding h or hh specifier, and
          displays 'am' for any hour before noon, and 'pm' for any hour
          after noon. The am/pm specifier can use lower, upper, or mixed
          case, and the result is displayed accordingly.

  a/p     Uses the 12-hour clock for the preceding h or hh specifier, and
          displays 'a' for any hour before noon, and 'p' for any hour after
          noon. The a/p specifier can use lower, upper, or mixed case, and
          the result is displayed accordingly.

  ampm    Uses the 12-hour clock for the preceding h or hh specifier, and
          displays the contents of the TimeAMString global variable for any
          hour before noon, and the contents of the TimePMString global
          variable for any hour after noon.

  /       Displays the date separator character given by the DateSeparator
          global variable.

  :       Displays the time separator character given by the TimeSeparator
          global variable.

  'xx'    Characters enclosed in single or double quotes are displayed as-is,
  "xx"    and do not affect formatting.

  Format specifiers may be written in upper case as well as in lower case
  letters--both produce the same result.

  If the string given by the Format parameter is empty, the date and time
  value is formatted as if a 'c' format specifier had been given.

  The following example:

    S := FormatDateTime('"The meeting is on" dddd, mmmm d, yyyy, ' +
      '"at" hh:mm AM/PM', StrToDateTime('2/15/95 10:30am'));

  assigns 'The meeting is on Wednesday, February 15, 1995 at 10:30 AM' to
  the string variable S. */

KYString FormatDateTime(const char* AFormat, TDateTime ADateTime);
KYString FormatDateTime(const KYString& AFormat, TDateTime ADateTime);

/* DateTimeToString converts the date and time value given by DateTime using
   the format string given by Format into the string variable given by Result.
   For further details, see the description of the FormatDateTime function. */

void DateTimeToString(KYString& Result, const char* AFormat, TDateTime ADateTime);

inline void DateTimeToString(KYString& Result, const KYString& AFormat,
                                                     TDateTime ADateTime)
{
   DateTimeToString(Result, (char*)AFormat, ADateTime);
}

}

#endif