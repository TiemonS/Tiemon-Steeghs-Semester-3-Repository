#include "BreadBaker.h"
#include "Program.h"
#include "TimeConstants.h"
#include "States.h"

#include <iostream>

BreadBaker::BreadBaker(
            IOven& oven, ITimer& timer, IKneadMotor& motor, IYeastTray& yeast,
            IExtraIngredientsTray& extras, IDisplay& display,
            IStartButtonLed& startButton, IEventGenerator& eventGenerator,
            Log& log)
    : oven(oven)
    , timer(timer)
    , motor(motor)
    , yeast(yeast)
    , extras(extras)
    , display(display)
    , startButton(startButton)
    , eventGenerator(eventGenerator)
    , log(log)
    , state(States::STANDBY)
{
}

bool BreadBaker::Pulse()
{
    auto ev = eventGenerator.GetEvent();
    if (ev != std::nullopt)
    {
        HandleEvent(*ev);
    }
    return ev != std::nullopt;
}

// parameter name in comment to prevent compiler warning as it is unused for now
void BreadBaker::HandleEvent(Events receivedEvent)
{
    //de groote switchcase die de verschillende events die binnenkomen verwerkt
    switch (receivedEvent)
    {
    //als is de menu button ingedrukt zal de state steeds wisselen tussen de verschillende programma's
    case MENU_BUTTON_PRESSED:
        switch (state)
        {
        case States::STANDBY:
            state = States::PROGRAM_PLAIN;
             std::cout << "Current program: PROGRAM_PLAIN" << "\n";
            break;
        case States::PROGRAM_PLAIN:
            state = States::PROGRAM_BREAD_PLUS;
             std::cout << "Current program: PROGRAM_BREAD_PLUS" << "\n";
            break;
        case States::PROGRAM_BREAD_PLUS:
            state = States::PROGRAM_RAPID;
            std::cout << "Current program: PROGRAM_RAPID" << "\n";
            break;
        case States::PROGRAM_RAPID:
            state = States::PROGRAM_DOUGH;
            std::cout << "Current program: PROGRAM_DOUGH" << "\n";
            break;
        case States::PROGRAM_DOUGH:
            state = States::PROGRAM_BAKE;
            std::cout << "Current program: PROGRAM_BAKE" << "\n";
            break;
        case States::PROGRAM_BAKE:
            state = States::STANDBY;
            std::cout << "Current program: STANDBY" << "\n";
            break;
        }
        break;
    default:
        break;
    }
}
