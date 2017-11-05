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
    void update() override;     //update is like void draw except with customizable framerate

    //my member functions
    void initializeFlies (std::vector <std::vector <int>> &theFlies);
    void drawFlies (std::vector <std::vector <int>> &theFlies, Graphics &g);
    void comparePixelValues (std::vector <std::vector <int>> &theFlies, std::vector <int> &theFitnesses);
    void updateFlies (std::vector <std::vector <int>> &theFlies, std::vector <int> &theFitnesses);
    
    

private:
    
    //image to analyze
    Image glass;
    
    //fly population
    int population;
    
    //random object
    Random r;
    
    //dimensions are x and y- 2d
    int dimensions;
    
    //lower and upper bounds for data space
    int lowerBounds;
    int upperBounds;
    
    //increasing this will randomly disperse a fly to look for a better solution
    float disturbanceThresh;
    
    //2d vector implementation for each fly- 1st dimension is which fly, 2nd are the x and y coordinates
    std::vector <std::vector <int>> theFlies;
    
    //stores the fitness scores for each fly so we move towards the best solution
    std::vector <int> theFitnesses;
    
    //how many times the fly checks random x and y positions to find symmetry
    int numTests;
    
    //the index of the fittest fly in the swarm
    int fittestInSwarm;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlyComponent)
};
