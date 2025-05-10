// // Set PJ0–PJ3 as input
// DDRJ &= ~((1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3));

// // Optional: enable pullups
// PORTJ |= (1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3);

// // Enable Pin Change Interrupt for Port J
// PCICR |= (1 << PCIE1);         // Enable PCINT group 1
// PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12);  // Enable specific pins


// volatile uint8_t last_state;

// ISR(PCINT1_vect) {
//     uint8_t current = PINJ & ((1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3));
//     uint8_t changed = current ^ last_state;
//     last_state = current;

//     if (changed & (1 << PJ0)) {
//         if (!(current & (1 << PJ0))) {
//             // Button on PJ0 pressed
//         }
//     }
//     if (changed & (1 << PJ1)) {
//         if (!(current & (1 << PJ1))) {
//             // Button on PJ1 pressed
//         }
//     }
//     // Add for PJ2, PJ3...
// }
