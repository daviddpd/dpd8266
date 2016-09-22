# ESP8266 using PWM code in 1.5.4+

## Note ##

This is a working, single GPIO pin PWM dimmer for an LED - dimming up/down.

There are a few details that are not at all clear in the English SDK API Reference.  I beat my head against them a few times to use the HW Timer PWM APIs.

### 1. pwm_init
```
void pwm_init( 
uint32 period,  
uint8 *duty,  
uint32 pwm_channel_num,   uint32 (*pin_info_list)[3])
```

It's not clear from the docmention, but the second argument, `uint8 *duty : duty cycle of each output` is actually an array of 32-bit intergers.

`uint32 pwm_channel_num: PWM channel number` in THIS FUNCTION, is not zero-indexed.  So if you have 1 channel, `pwm_channel_num=1`.  Beat my head against this setting it to zero for a single channel config.



### 3. pwm_set_duty

Unlike pwm_init, channel is zero based.  So with one channel defined, this is channel=0.  This really confused me for a few hours.


