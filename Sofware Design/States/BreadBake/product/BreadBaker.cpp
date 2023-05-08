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
        switch (programNumber)
        {
        case 1:
            //display.SetMenu(programNumber);
            std::cout << "Current program: " << programNumber << " - PROGRAM_PLAIN " << "Lenght: 4 u 50 min"<<"\n";
            programNumber++;
            break;
        case 2:
            std::cout << "Current program: " << programNumber << " - PROGRAM_BREAD_PLUS " << "Lenght: 4 u 50 min"<<"\n";
            programNumber++;
            break;
        case 3:
            std::cout << "Current program: " << programNumber << " - PROGRAM_RAPID " << "Lenght: 1 u 55 min"<<"\n";
            programNumber++;
            break;
        case 4:
            std::cout << "Current program: " << programNumber << " - PROGRAM_DOUGH " << "Lenght: 2 u 20 min"<<"\n";
            programNumber++;
            break;
        case 5:
            std::cout << "Current program: " << programNumber << " - PROGRAM_BAKE " << "Lenght: 30 - 90 min"<<"\n";
            programNumber = 1;
            break;
        }
        break;
    case TIMER_UP_BUTTON_PRESSED:
        if (state = STANDBY)
        {
            timerTime += 10 MIN;
        }
        else if(state = PROGRAM_BAKING) 
        {
            bakingTime += 10 MIN;
        }
          
        break;
    case TIMER_DOWN_BUTTON_PRESSED:
        if (state = STANDBY)
        {
            timerTime -= 10 MIN;
        }
        else if(state = PROGRAM_BAKING) 
        {
            bakingTime -= 10 MIN;
        }
        break;
    case MENU_BUTTON_LONG_PRESSED:
        state = States::STANDBY;
        timer.Cancel();
        oven.Cancel();
        break;
    //start knop ingedrukt
    case START_BUTTON_PRESSED:
        state = States::IN_PROGRAM;
        //de start methode uitvoeren waarin word gekeken naar welk programma was uitgekozen
        StartProgram(&selectedProgram);
        break;
    case TIMER_TIMEOUT:
        //elke keer als de timer voorbij is wordt het programma geupdate
        UpdateProgram(&selectedProgram);
        break;
    case OVEN_DONE:
        //als de oven klaar is met rising of baking wordt de updateprogram methode weer aangeroepen
        UpdateProgram(&selectedProgram);
    default:
        break;
    }
}

void BreadBaker::StartProgram(Program *selectedProgram) 
{
        switch (programNumber - 1)
        {
        case 1:
            //PROGRAM_PLAIN
            selectedProgram->waiting = 60 MIN;
            selectedProgram->kneading = 20 MIN;
            selectedProgram->rising = 160 MIN;
            selectedProgram->baking = 50 MIN;
            selectedProgram->addYeast = false;
            selectedProgram->addExtras = false;
            state = PROGRAM_WAITING;
            display.SetCurrentTask(WAITING);
            break;
        case 2:
            //PROGRAM_BREAD_PLUS
            selectedProgram->waiting = 60 MIN;
            selectedProgram->kneading = 20 MIN;
            selectedProgram->rising = 160 MIN;
            selectedProgram->baking = 50 MIN;
            selectedProgram->addYeast = false;
            selectedProgram->addExtras = false;
            state = PROGRAM_WAITING;
            display.SetCurrentTask(WAITING);
            break;
        case 3:
            //PROGRAM_RAPID
            selectedProgram->waiting = 0 MIN;
            selectedProgram->kneading = 15 MIN;
            selectedProgram->rising = 60 MIN;
            selectedProgram->baking = 40 MIN;
            selectedProgram->addYeast = false;
            selectedProgram->addExtras = false;
            state = PROGRAM_KNEADING;
            display.SetCurrentTask(KNEADING);
            break;
        case 4:
            //PROGRAM_DOUGH
            selectedProgram->waiting = 40 MIN;
            selectedProgram->kneading = 20 MIN;
            selectedProgram->rising = 80 MIN;
            selectedProgram->baking = 0 MIN;
            selectedProgram->addYeast = false;
            selectedProgram->addExtras = false;

            state = PROGRAM_WAITING;
            display.SetCurrentTask(WAITING);
            break;
        case 5:
            //PROGRAM_BAKE
            selectedProgram->waiting = 0 MIN;
            selectedProgram->kneading = 0 MIN;
            selectedProgram->rising = 0 MIN;
            selectedProgram->baking = timerTime MIN;
            selectedProgram->addYeast = false;
            selectedProgram->addExtras = false;
            state = PROGRAM_BAKING;
            display.SetCurrentTask(WAITING);
            break;
        default:
            break;
        }
        timer.Set(selectedProgram->waiting); 
}

/* 
De methode voor het updaten van het geselecteerde programma.
Er wordt steeds gekeken in welk stadia het programma zich bevindt en aan de hand daarvan wordt er gehandeld.
*/
void BreadBaker::UpdateProgram(Program *selectedProgram) 
{
    switch (state)
    {
    case PROGRAM_WAITING:
        timer.Set(0);
        display.SetCurrentTask(KNEADING);
        state = PROGRAM_KNEADING;
        break;
    case PROGRAM_KNEADING:
    //Het knead programma draait steeds 1 min naar link en 1 min naar rechts
    //totdat het totaal aantal minuten berijkt is
        std::cout << kneadCounter << "\n";
        if(kneadCounter == selectedProgram->kneading / 60000) //elke knead duurt 1 min hier wordt het totaal vergeleken
        {
            timer.Set(0);
            kneadCounter = 0;
            display.SetCurrentTask(RISING);
            state = PROGRAM_RISING;
        }  
        else if (kneadCounter % 2 == 0) //is het kneadcounter een even getal?, turn left
        {
            timer.Set(60000);
            motor.TurnLeft();
            kneadCounter++;
        }
        else //anders turn right
        {
            timer.Set(60000);
            motor.TurnRight();
            kneadCounter++;
        }
        break;
    case PROGRAM_RISING:
        oven.StartRise(selectedProgram->rising); 
        display.SetCurrentTask(RISING);

        //kijken of het geselecteerde programma een baking fase heeft
        if (selectedProgram->baking <= 0)
        {   
            state = PROGRAM_DONE;
        }
        else 
        {
            state = PROGRAM_BAKING; 
        }
        break;
    case PROGRAM_BAKING:
        oven.StartBake(selectedProgram->baking);
        display.SetCurrentTask(BAKING);
        state = PROGRAM_DONE; 
        break;
    case PROGRAM_DONE:
        display.SetCurrentTask(DONE);
        break;
    default:
        break;
    }
}

//TODO

//Weergave van het start van elk programma fixen

//State diagram updaten

