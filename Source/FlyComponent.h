/*
  ==============================================================================

    FlyComponent.h
    Created: 5 Nov 2017 10:40:14am
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class FlyComponent    : public AnimatedAppComponent
{
public:
    FlyComponent();
    ~FlyComponent();

    void paint (Graphics&) override;
    void resized() override;
    void update() override;

    
    void initializeFlies (std::vector <std::vector <int>> &theFlies);
    void drawFlies (std::vector <std::vector <int>> &theFlies, Graphics &g);
    void comparePixelValues (std::vector <std::vector <int>> &theFlies, std::vector <int> &theFitnesses);
    void updateFlies (std::vector <std::vector <int>> &theFlies, std::vector <int> &theFitnesses);
    
    

private:
    Image glass;
    
    int population;
    
    //random object
    Random r;
    
    int dimensions;
    int lowerBounds;
    int upperBounds;
    float disturbanceThresh;
    std::vector <int> fly;
    std::vector <std::vector <int>> theFlies;
    std::vector <int> theFitnesses;
    
    std::vector <int> fitness;
    float flyFitness;
    int fittestInSwarm;
    int iterations;
    int numTrials = 25;
    
    int posX, posY;
    
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlyComponent)
};
