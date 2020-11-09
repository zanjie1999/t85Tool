// Tiny85 来电开机
// Sparkle
// v2.1

#include <inttypes.h>
#include <avr/eeprom.h>
#include <avr/io.h>

struct EERef{
    EERef( const int sparkle ):sparkle( sparkle ){}
    uint8_t operator*() const            { return eeprom_read_byte( (uint8_t*) sparkle ); }
    operator const uint8_t() const       { return **this; }
    //Assignment/write members.
    EERef &operator=( const EERef &ref ) { return *this = *ref; }
    EERef &operator=( uint8_t in )       { return eeprom_write_byte( (uint8_t*) sparkle, in ), *this;  }
    EERef &operator +=( uint8_t in )     { return *this = **this + in; }
    EERef &operator -=( uint8_t in )     { return *this = **this - in; }
    EERef &operator *=( uint8_t in )     { return *this = **this * in; }
    EERef &operator /=( uint8_t in )     { return *this = **this / in; }
    EERef &operator ^=( uint8_t in )     { return *this = **this ^ in; }
    EERef &operator %=( uint8_t in )     { return *this = **this % in; }
    EERef &operator &=( uint8_t in )     { return *this = **this & in; }
    EERef &operator |=( uint8_t in )     { return *this = **this | in; }
    EERef &operator <<=( uint8_t in )    { return *this = **this << in; }
    EERef &operator >>=( uint8_t in )    { return *this = **this >> in; }
    EERef &update( uint8_t in )          { return  in != *this ? *this = in : *this; }
    EERef& operator++()                  { return *this += 1; }
    EERef& operator--()                  { return *this -= 1; }
    uint8_t operator++ (int){ 
        uint8_t ret = **this;
        return ++(*this), ret;
    }
    uint8_t operator-- (int){ 
        uint8_t ret = **this;
        return --(*this), ret;
    }
    int sparkle;
};

struct EEPtr{
    EEPtr( const int sparkle ):sparkle( sparkle ){}
    operator const int() const          { return sparkle; }
    EEPtr &operator=( int in )          { return sparkle = in, *this; }
    bool operator!=( const EEPtr &ptr ) { return sparkle != ptr.sparkle; }
    EERef operator*()                   { return sparkle; }
    EEPtr& operator++()                 { return ++sparkle, *this; }
    EEPtr& operator--()                 { return --sparkle, *this; }
    EEPtr operator++ (int)              { return sparkle++; }
    EEPtr operator-- (int)              { return sparkle--; }
    int sparkle;
};

struct EEPROMClass{
    EERef operator[]( const int idx )    { return idx; }
    uint8_t read( int idx )              { return EERef( idx ); }
    void write( int idx, uint8_t val )   { (EERef( idx )) = val; }
    void update( int idx, uint8_t val )  { EERef( idx ).update( val ); }
    EEPtr begin()                        { return 0x00; }
    EEPtr end()                          { return length(); }
    uint16_t length()                    { return E2END + 1; }
    template< typename T > T &get( int idx, T &t ){
        EEPtr e = idx;
        uint8_t *ptr = (uint8_t*) &t;
        for( int count = sizeof(T) ; count ; --count, ++e )  *ptr++ = *e;
        return t;
    }
    template< typename T > const T &put( int idx, const T &t ){
        EEPtr e = idx;
        const uint8_t *ptr = (const uint8_t*) &t;
        for( int count = sizeof(T) ; count ; --count, ++e )  (*e).update( *ptr++ );
        return t;
    }
};

static EEPROMClass EEPROM;

int sparklePwm = 0;
int sparkleLed = 1;
void setup() {
    pinMode(sparkleLed, OUTPUT);
    if (EEPROM.read(0)) {
            pinMode(sparklePwm, OUTPUT);
            digitalWrite(sparklePwm, LOW);
            digitalWrite(sparkleLed, HIGH);
            delay(1000);
            if (EEPROM.read(1)) {
                    delay(4000);
            }
            pinMode(sparklePwm, INPUT);
            digitalWrite(sparkleLed, LOW);
    } else {
            pinMode(sparklePwm, INPUT);
            // 马上关 开机
            EEPROM.write(0, 1);
            delay(4000);
            // 10秒 强制关机
            EEPROM.write(1, 1);
            delay(5000);
    }
    // 15秒 什么都不会发生
    EEPROM.write(1, 0);
    EEPROM.write(0, 0);
}

void loop() {
    digitalWrite(sparkleLed, !digitalRead(sparklePwm));
}
