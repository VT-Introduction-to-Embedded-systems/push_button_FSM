// This application uses button 1, LED 1, and LED 2 on Launchpad
// Pushing button 1 toggles LED1 status.
// Pushing a button consists of "pressing, followed by releasing".
// This a typical push-button behavior: as soon as you press the button, LED toggles,
// as you keep the button pressed nothing happens, once you release it, if you press again, the LED toggles again.

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// According to the schematics on page 37 of the Launchpad user guide,
// When a button is pressed, it is grounded (logic 0)
#define PRESSED 0

// This function initializes all the peripherals
void initialize();

void TurnOn_Launchpad_LED1();
void TurnOff_Launchpad_LED1();
void Toggle_Launchpad_LED1();

// This function returns true if the button1 is pressed
bool Launchpad_Button1_pressed();

// This is the raw status of the button (0 or 1)
char SwitchStatus_Launchpad_Button1();

// The type that holds the states of the button for its FSM
typedef enum {PUSHED, RELEASED} button_state_t;

int main(void)
{
    initialize();

    // The state of button1
    button_state_t button1_state = RELEASED;

    // The output of the FSM
    bool do_toggle_LED1 = false;

    while (1) {

        switch(button1_state)
        {
        case PUSHED:

            // transition arc from PUSHED to RELEASED
            if (!Launchpad_Button1_pressed())
            {
                //output of the arc
                do_toggle_LED1 = false;

                // next state
                button1_state = RELEASED;
            }
            break;

        // transition arc from RELEASED to PUSHED
        case RELEASED:
            if (Launchpad_Button1_pressed())
            {
                //output of the arc
                do_toggle_LED1 = true;

                // next state
                button1_state = PUSHED;
             }
        }

        if (do_toggle_LED1)
            Toggle_Launchpad_LED1();

    }
}


// Initialization part is always device dependent and therefore does not change much with HAL
void initialize()
{

    // step 1: Stop watchdog timer
    // We do this at the beginning of all our programs for now.Later we learn more about it.
    WDT_A_hold(WDT_A_BASE);

    // step 2: Initializing Launchpad LED1, which is on Pin 0 of Port P1 (from page 37 of the Launchpad User Guide)
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    // step 3: Initializing Launchpad S1 (switch 1 or button 1),
    // which is on Pin1 of Port 1 (from page 37 of the Launchpad User Guide)
    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P1, GPIO_PIN1);

}


void TurnOn_Launchpad_LED1()
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
void TurnOff_Launchpad_LED1()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
void Toggle_Launchpad_LED1()
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

char SwitchStatus_Launchpad_Button1()
{
    return (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1));
}

bool Launchpad_Button1_pressed()
{
    return (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == PRESSED);
}
