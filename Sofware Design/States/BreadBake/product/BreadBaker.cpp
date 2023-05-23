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

void BreadBaker::HandleStandbyState(Events receivedEvent) 
{
    //de groote switchcase die de verschillende events die binnenkomen verwerkt
    switch (receivedEvent)
    {
    //als is de menu button ingedrukt zal de state steeds wisselen tussen de verschillende programma's
    case MENU_BUTTON_PRESSED:
        switch (programNumber)
        {
        case 0:
            //display.SetMenu(programNumber);
            programNumber++;
            std::cout << "Current program: " << programNumber << " - PROGRAM_BREAD_PLUS " << "Lenght: 4 u 50 min"<<"\n";
            break;
        case 1:
            programNumber++;
            std::cout << "Current program: " << programNumber << " - PROGRAM_RAPID " << "Lenght: 1 u 55 min"<<"\n";
            break;
        case 2:
            programNumber++;
            std::cout << "Current program: " << programNumber << " - PROGRAM_DOUGH " << "Lenght: 2 u 20 min"<<"\n";
            break;
        case 3:
            programNumber++;
            std::cout << "Current program: " << programNumber << " - PROGRAM_BAKE " << "Lenght: 30 - 90 min"<<"\n";
            break;
        case 4:
            programNumber = 0;
            std::cout << "Current program: " << programNumber << " - PROGRAM_PLAIN " << "Lenght: 4 u 50 min"<<"\n";
            break;
        }
        break;
    case TIMER_UP_BUTTON_PRESSED:
        //het instellen van de tijd voor het bak programma of de timer vooraf
        //de timer mag niet langer zijn dan 12 uur = 720 min
        //de bakingtime moet tussen de 30-90 min zijn
        if (programNumber == 4 && bakingTime < 90)
        {   
            bakingTime += 10;
        }
        else if(timerTime < 720)
        {
            timerTime += 10;
        }       
        std::cout << "BakingTime: "<< bakingTime<< "\n";
        std::cout << "TimerTime: "<< timerTime<< "\n";
        break;
    case TIMER_DOWN_BUTTON_PRESSED:
        if (programNumber == 4 && bakingTime > 30)
        {   
            bakingTime -= 10;
        }
        else if(timerTime >= 0)
        {
            timerTime -= 10;
        }  
        std::cout << "BakingTime: "<< bakingTime << "\n";
        std::cout << "TimerTime: "<< timerTime << "\n";
        break;
    //start knop ingedrukt
    case START_BUTTON_PRESSED:
        //de start methode uitvoeren waarin word gekeken naar welk programma was uitgekozen
        StartProgram(&selectedProgram);
        break;
    default:
        break;
    }
}

void BreadBaker::HandleWaitingState(Events receivedEvent) 
{
    switch (receivedEvent)
    {
    case TIMER_TIMEOUT:
        timer.Set(0);
        state = PROGRAM_KNEADING;
        break;
    default:
        break;
    }
}

void BreadBaker::HandleKneadingState(Events receivedEvent) 
{
    switch (receivedEvent)
    {
    case TIMER_TIMEOUT:
        //Het knead programma draait steeds 1 min naar link en 1 min naar rechts
        //totdat het totaal aantal minuten bereikt is
        std::cout << kneadCounter << "\n";
        if(kneadCounter == selectedProgram.kneading / 60000) //elke knead duurt 1 min hier wordt het totaal vergeleken
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
    default:
        break;
    }
}

void BreadBaker::HandleRisingState(Events receivedEvent) 
{
    //als de oven klaar is de de kneading fase zal hij in de rising state gaan
    switch (receivedEvent)
    {
    case TIMER_TIMEOUT:
        oven.StartRise(selectedProgram.rising); 

        //kijken of het geselecteerde programma een baking fase heeft
        if (selectedProgram.baking <= 0)
        {   
            state = PROGRAM_DONE;
        }
        else 
        {
            state = PROGRAM_BAKING; 
        }
        break;
    default:
        break;
    }
}

void BreadBaker::HandleBakingState(Events receivedEvent) 
{
    //wanneer de oven klaar is met de rising state zal de oven in de baking state gaan
    switch (receivedEvent)
    {
    case OVEN_DONE:
        oven.StartBake(selectedProgram.baking);
        state = PROGRAM_DONE; 
        break;
    
    default:
        break;
    }

    bakingTime -= 10 MIN;
}

void BreadBaker::HandleDoneState(Events receivedEvent)  
{
    switch (receivedEvent)
    {   
    case OVEN_DONE:
        state = STANDBY; 
        break;
    default:
        break;
    }
}

/* 
De HandleEvent methode die wordt aangeroepen op het moment dat er een event plaats vindt.
Deze methode verwerkt de events door steeds te kijken naar in welke state de machine zich bevdint
en dan aan de hand daarvan te handelen.
*/
void BreadBaker::HandleEvent(Events receivedEvent)
{
    if (receivedEvent == MENU_BUTTON_LONG_PRESSED)
    {
        state = States::STANDBY;
        timer.Cancel();
        oven.Cancel();
    }
    
    switch (state)
    {
    case STANDBY:
        HandleStandbyState(receivedEvent);
        break;
    case PROGRAM_WAITING:
        display.SetCurrentTask(WAITING);
        HandleWaitingState(receivedEvent);
        break;
    case PROGRAM_KNEADING:
        //Het knead programma draait steeds 1 min naar link en 1 min naar rechts
        //totdat het totaal aantal minuten berijkt is
        display.SetCurrentTask(KNEADING);
        HandleKneadingState(receivedEvent);
        break;
    case PROGRAM_RISING:
        display.SetCurrentTask(RISING);
        HandleRisingState(receivedEvent);
        break;
    case PROGRAM_BAKING:
        display.SetCurrentTask(BAKING);
        HandleBakingState(receivedEvent);
        break;
    case PROGRAM_DONE:
        display.SetCurrentTask(DONE);
        HandleDoneState(receivedEvent);
        break;
    default:
        break;
    }
}

void BreadBaker::StartProgram(Program *selectedProgram) 
{
        switch (programNumber)
        {
        case 0:
            //PROGRAM_PLAIN
            selectedProgram->waiting = 60 MIN;
            selectedProgram->kneading = 20 MIN;
            selectedProgram->rising = 160 MIN;
            selectedProgram->baking = 50 MIN;
            selectedProgram->addYeast = false;
            selectedProgram->addExtras = false;
            state = PROGRAM_WAITING;
            break;
        case 1:
            //PROGRAM_BREAD_PLUS
            selectedProgram->waiting = 60 MIN;
            selectedProgram->kneading = 20 MIN;
            selectedProgram->rising = 160 MIN;
            selectedProgram->baking = 50 MIN;
            selectedProgram->addYeast = false;
            selectedProgram->addExtras = false;
            state = PROGRAM_WAITING;
            break;
        case 2:
            //PROGRAM_RAPID
            selectedProgram->waiting = 0 MIN;
            selectedProgram->kneading = 15 MIN;
            selectedProgram->rising = 60 MIN;
            selectedProgram->baking = 40 MIN;
            selectedProgram->addYeast = false;
            selectedProgram->addExtras = false;
            state = PROGRAM_KNEADING;
            break;
        case 3:
            //PROGRAM_DOUGH
            selectedProgram->waiting = 40 MIN;
            selectedProgram->kneading = 20 MIN;
            selectedProgram->rising = 80 MIN;
            selectedProgram->baking = 0 MIN;
            selectedProgram->addYeast = false;
            selectedProgram->addExtras = false;

            state = PROGRAM_WAITING;
            break;
        case 4:
            //PROGRAM_BAKE
            selectedProgram->waiting = 0 MIN;
            selectedProgram->kneading = 0 MIN;
            selectedProgram->rising = 0 MIN;
            selectedProgram->baking = timerTime MIN;
            selectedProgram->addYeast = false;
            selectedProgram->addExtras = false;
            state = PROGRAM_BAKING;
            break;
        default:
            break;
        }
        timer.Set(selectedProgram->waiting); 
}

//TODO


//State diagram updaten

//nieuwe logica afmaken en ook hiervoor de state diagram updaten
//door eerst te kijken naar in welke state de machine zich bevind wordt er bijvoorbeeld niet gekeken naar de start menu knop
//tijdens de kneading fase

