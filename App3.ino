// App3.ino
//

volatile uint32_t _v = 0;
volatile uint32_t _t = 0;

uint32_t _ts[512] = { 0 };
uint8_t _tt[512] = { 0 };
uint32_t t = 0;

void _ISR()
{
    if (_v < sizeof(_ts) / sizeof(uint32_t))
    {
        t = micros() - _t;

        if (!_t || t < 100)
        {
            _tt[_v] = 'M';
            ++(_ts[_v]);
        }
        else
        {
            _tt[++_v] = 'S';
            _ts[_v++] = t;
        }
    }

    _t = micros();
}

void setup()
{
    Serial.begin(115200);
    while (!Serial.available());
    Serial.println("setup()");

    pinMode(A0, INPUT_PULLDOWN);
    attachInterrupt(A0, _ISR, RISING);
}

uint8_t _buf[512] = { 0 };

void loop()
{
    if (Serial.available())
    {
        uint8_t k = Serial.read();
        if (k == 'p')
        {
            Serial.print("_v = ");
            Serial.println(_v);
            Serial.print("/2 = ");
            Serial.println(_v / 2);

            for (uint32_t i = 0; i < _v; i++)
            {
/*
                if ((!(i % 2) && _tt[i] != 'M') || ((i % 2) && _tt[i] != 'S'))
                    Serial.print('**E**');

                //Serial.print((char)_tt[i]);
                Serial.print(_tt[i] == 'M' ? _ts[i] : (uint32_t)(_ts[i] / 26.701632 + 0.5));

                if (i < _v - 1)
                    Serial.print(",");
*/
                if (i % 2)
                {
                    uint32_t m = _ts[i - 1];
                    uint32_t s = (uint32_t)(_ts[i] / 26.701632 + 0.5);
 
                    //sprintf((char *)_buf, "m=%d,s=%d|", m, s);
                    //Serial.print((const char *)_buf);

                    if ((m > 20 && m < 26) && (s > 19 && s < 25))
                        Serial.print('0');
                    else if ((m > 20 && m < 26) && (s > 60 && s < 66))
                        Serial.print('1');
                    else if ((m > 20 && m < 26) && (s > 742 && s < 748))
                        Serial.print('2');
                    else if ((m > 333 && m < 339) && (s > 165 && s < 171))
                        Serial.print('S');
                    else
                        Serial.print('?');
                }
            }

            Serial.println();

            for (uint32_t i = 0; i < _v; i++)
                _ts[i] = _tt[i] = 0;

            _v = _t = 0;
        }
    }
}
