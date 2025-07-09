# include "motor.h"

motor::motor(unsigned long microstep) : microstep(microstep) {
}

void motor::
initMotor() {
    initPWM_TIM1();
    initCounter_TIM5();
    // Set PH2 to PH3 as output
    DDRH |= (1 << PH6) | (1 << PH5) | (1 << PH4) | (1 << PH3) ; // DIR+, DIR-, ENA+, ENA-

    // Optionally set them low or high
    PORTH &= ~((1 << PH6) | (1 << PH5) | (1 << PH4) | (1 << PH3)); // All low

}

void motor::DISmotor() {
    PORTH = (PORTH & ~(1 << PH3)) | (1 << PH4); // Set EN+ high, EN- low
    running = false; // Set motor state to stopped

}

void motor::ENmotor() {
    PORTH = (PORTH & ~(1 << PH4)) | (1 << PH3); // Set EN- low, EN+ high
    running = true; // Set motor state to running
}
 
void motor::chanageDIR_ccw() {
    PORTH = (PORTH & ~(1 << PH6)) | (1 << PH5); // Set DIR+ low, DIR- high
    turn_direction = false; // Set direction to counterclockwise
}

void motor::chanageDIR_cw() {
    PORTH = (PORTH & ~(1 << PH5)) | (1 << PH6); // Set DIR+ high, DIR- low
    turn_direction = true; // Set direction to clockwise
}

void motor::initPWM_TIM1() {
    DDRB |= (1 << PB5);    // Set OC1A (PB5) as output

    // Set CTC mode: WGM12 = 1 (Mode 4), toggle OC1A on match
    TCCR1A = (1 << COM1A0);                     // Toggle OC1A on compare match
    TCCR1B = (1 << WGM12) | (1 << CS11);        // CTC mode, Prescaler = 8

    // Calculate and set OCR1A for desired frequency
}

void motor::initCounter_TIM5() {

    DDRL &= ~(1 << PL2);      // Set T5 (PL2) as input

    // Clear Timer on Compare Match mode (CTC)
    TCCR5B = (1 << WGM52);  // CTC with OCR5A

    // Clear timer
    TCNT5 = 0;

    sei();  // Enable global interrupts
}

void motor::stopCounter_TIM5() {
    TCCR5B = 0; // Stop counter
}

void motor::stopPWM_TIM1() {
    TCCR1A = 0; // Stop PWM
    OCR1A = 0; // Reset compare value
}

void motor::startCounter_TIM5() {
    TCCR5B |= (1 << CS52) | (1 << CS51) | (1 << CS50); // Start counter
}

void motor::startPWM_TIM1() {
    TCCR1A |= (1 << COM1A0); // Start PWM
}

void motor::resetCounter_TIM5() {
    TCNT5 = 0; // Reset counter
    OCR5A = 0; // Reset compare value
}

void motor::resetPWM_TIM1() {
    OCR1A = 0; // Reset compare value
}

void motor::detachINTERUPT_TIM5() {
    TIMSK5 &= ~(1 << OCIE5A); // Disable interrupt
}

void motor::attachINTERUPT_TIM5() {
    TIMSK5 |= (1 << OCIE5A); // Enable interrupt
}


void motor::setSpeed(unsigned int rpm) {
    if (rpm > 1000) {
        rpm = 1000;
    }else if(rpm == 0){
        stopPWM_TIM1(); // Stop PWM if RPM is 0
        return; // Exit if RPM is 0
    }else if(current_rpm == 0 ){
        startPWM_TIM1(); // Start PWM if RPM is not 0 and was previously 0
    }
    uint32_t frequency_hz = rpm * microstep / 60; // Convert RPM to frequency in Hz

     // Calculate OCR1A value
     uint32_t ocr_val = (F_CPU / (2 * PRESCALER * frequency_hz)) - 1;
     if (ocr_val > 65535) {
         ocr_val = 65535; // Limit to 16-bit value
     }
 
    OCR1A = ocr_val; // Set the PWM duty cycle

}

void motor::setDirection(bool dir) {
    if (dir) {
        chanageDIR_cw(); // Set direction to clockwise
    } else {
        chanageDIR_ccw(); // Set direction to counterclockwise
    }
    // direction = dir; // Update direction variable
}

void motor::setAngle(unsigned int angle) {

    resetCounter_TIM5(); // Reset counter
    uint32_t count = angle * microstep / 360; // Convert angle to count
    if(count > 65536) { // 2^16 
        count = 65536; // Limit to 16-bit value
        //Error handling: angle too large
    }
    OCR5A = count - 1;  // Zero-indexed
    attachINTERUPT_TIM5(); // Attach interrupt
}

void motor::stopMotor() { 
    stopCounter_TIM5(); // Stop counter
    resetCounter_TIM5(); // Reset counter
    stopPWM_TIM1(); // Stop PWM
    resetPWM_TIM1(); // Reset compare value
    detachINTERUPT_TIM5(); // Detach interrupt
    DISmotor(); // Disable motor
    last_count = 0; // Reset last count
    turn_direction = false;
}

void motor::runMotor() {

    startCounter_TIM5(); // Start counter
    startPWM_TIM1(); // Start PWM

    if(!safety_on){ENmotor();} // Enable motor
    // Set motor state to running
}

void motor::speedcontrol(int rpm) {

    if(rpm > 0 ) {
        setDirection(true); // Set direction to forward
    } else if(rpm < 0 ) {
        setDirection(false); // Set direction to backward
    }
    if(rpm < 0) {
        rpm = -rpm; // Make RPM positive
    }
    setSpeed(rpm); // Set speed
    current_rpm = rpm;
    if(!running && rpm != 0) {
        runMotor(); // Start motor if not already running
    }
}

void motor::turnAngle(long int angle, unsigned int rpm) {
    if(angle < 0) {
        rpm = -rpm; // Make RPM negative for backward direction
        angle = -angle; // Make angle positive
    }
    setAngle(angle); // Set angle
    speedcontrol(rpm); // Set speed
}

double motor::getAngle() {
    unsigned long current_count = TCNT5; // Read the current counter value
    double angle = double(current_count * 360) / microstep; // Convert count to angle
    return angle; // Return the angle
}

void motor::resetAngle() {
    resetCounter_TIM5(); // Reset counter
    angle_set = false; // Reset angle set flag
}

void motor::motorSafetyEN() {

    if ((safety_count > 0 || !turn_direction) ){
        int diff = TCNT5 - last_count; // Calculate the difference in counts
        last_count = TCNT5; // Update last count

        if (diff < 0) {
            diff += 65536; // Handle overflow
        }
        if(turn_direction){
            safety_count = safety_count - diff; // Decrement safety count based on the number of turns
        }else {
            safety_count = safety_count + diff; // Increment safety count based on the number of turns
        }
        safety_on = false; 
    } 
    if(safety_count <= 0) {
        stopMotor();
        safety_count = 0; // Reset safety count
        safety_on = true; // Enable safety feature
    }
}

long int motor::getsafetyCount() {
    return safety_count; // Return the current safety count
}

int motor::getCurrentRpm(){
    return current_rpm; // Return the current RPM
}

bool motor::saveSafetyToEEPROM(EEPROMManager *eeprom) {

    if( ((safety_count - last_saved_safety_count) > 500) || ((last_saved_safety_count - safety_count) > 500)) {
        save_safety_to_eeprom = true;
        last_saved_safety_count = safety_count;
    }else if(safety_count == 0 && !safety_written_for_zero) {
        save_safety_to_eeprom = true; // Save safety count if it is zero
        last_saved_safety_count = 0; // Reset last saved safety count
        safety_written_for_zero = true; // Set flag to indicate safety count is written for zero
    }else{
        save_safety_to_eeprom = false; // Reset flag if no significant change
    }

    if(save_safety_to_eeprom) {
        eeprom->store("MSC1", safety_count); // Store safety count in EEPROM
        save_safety_to_eeprom = false; // Reset flag after saving
        return true; // Return true if saved successfully
    }
    return false; // Return false if not saved
}

void motor::setSafetyCount(EEPROMManager *eeprom) {
    uint16_t val;
    if(eeprom->read("MSC1", &val)) {
        safety_count = val; // Read safety count from EEPROM
    }else{
        safety_count = 30000; // Default safety count if not found in EEPROM
    }
    last_saved_safety_count = safety_count; // Initialize last saved safety count
}
