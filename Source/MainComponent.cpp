/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

/*
 The Main Component just holds a place for the rest of this project to reside....for the real implementation check out FlyComponent.cpp and .h
 */



MainContentComponent::MainContentComponent()
{
    
    //Setting sizes and adding FlyComponent as a child of Main Component
    setSize (450, 450);
    addAndMakeVisible(&flyComponent);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
   
}

void MainContentComponent::resized()
{
    //Bounds are set to same dimensions as main window
    flyComponent.setBounds(getLocalBounds());
}


