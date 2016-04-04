BrainStudio Tutorial Exercises
==============================

These exercises will take you through a basic tour of the basic set of
BrainStudio features. You will implement a few simple neural systems and study
their behaviour.

[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Ex1: Single PING oscillator

## Introduction

Pyramidal Inter-Neuron Gamma (PING) oscillators are believed to be a major
source of oscillations in the cortex. The simplest generation mechanism
involves two populations of neurons, an excitatory and an inhibitory one.
Both populations are strongly connected to themselves and weakly connected
to the other.

The basic dynamics work as follows: some incoming current makes some
excitatory neurons fire, which excites other excitatory neurons and also the
inhibitory ones. This, in turn, inhibits the excitatory population a bit
later, shutting down inhibition and letting the current act again. This
creates an oscillation with a frequency that depends on the parameters of the
neurons and the synapses, but is typically around 40Hz.

## What you'll learn

In this exercise you will use the basic features of BrainStudio, like adding
nodes and edges, choosing neuron models and parameter sets and running
simulations. You will also use BrainStudio's stimulation widget to inject
current into the populations and observe a range of emergent dynamical
phenomena.

## What to do

To complete this exercise, you should:

1. Create a population of 800 Izhikevich neurons, with the 'Excitatory'
parameter set and another population of 200 Izhikevich neurons with the
'Inhibitory' parameter set.
  * *Hint*: You can add nodes from the menu in the top left or with a double
  click anywhere in the main window. Choose first a neuron model and then one
  of the parameter sets available for that model, or write your own.
2. Add a connection from the excitatory to inhibitory population, another
connection from the inhibitory to the excitatory, and a recurrent connection
within the inhibitory. Use a delay of 6ms for all synapses.
  * *Hint*: You can add synapses by clicking and dragging on the '+' button
  in each node. You can use formulas and functions inside the definition of
  the connections, including random number generators like `RANDF`. Try using
  a weight of `2*RANDF()` for the excitatory synapses and `-2*RANDF()` for the
  inhibitory ones.
3. Add a recurrent connection within the excitatory module with stronger
weights (distributed randomly between 1 and 17) and with a density of 1% (Try
changing the connectivity pattern when adding the synapse).
4. Load the network in NeMo and run it using the buttons in the toolbar. You
can use the 'Stimulation' button to start the stimulation widget and inject
current into the modules. You can explore different visualizations in the menu
at the bottom left.

Now you can experiment in real time with the stimulation (you can stimulate
one or both populations, with varying intensity) and the neuron parameters.
Under the right conditions, you should see the excitatory group oscillate
with a frequency in the gamma band. Congratulations, that's your first PING
oscillator!

<u>Extra</u>: Try multiple configurations of stimulations and parameters. Can
you spot different dynamical regimes? What happens if you inject a current
that is also oscillating? How does the frequency of the stimulation affect the
behaviour of the population?


[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Ex2: Uncoupled PING pair

## Introduction

The complex oscillatory patterns we observe in cortical Local Field Potentials
(LFP) and EEG cannot be accounted for with one type of oscillator only. As
mentioned in exercise 1, the parameters of the neurons and the connections
change heavily the behaviour of the PING.

The parameter that affects the frequency the most is the synaptic delay --
i.e. how much does information take to go back and forth between the
populations.

## What you'll learn

In this exercise you'll learn about other features and shortcuts to make
network design more convenient.

## What to do

To complete this exercise, you should:

1. Start from your previous PING brn file.
    * *Hint*: Look at the menus in the top left and use the keyboard shortcuts
    to open, load and save brn files.
2. Make a copy of both modules and replicate the connections between them,
this time with a delay of 4ms in all synapses.
    * *Hint*: You can use BrainStudio's Copy+Paste shortcuts to save you time.
3. Run the network as you did with the single PING.


[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Ex3: Coupled PING pair

## Introduction

A leading theory of how different neural groups communicate amidst a sea
of uncontrolled oscillations is Pascal Fries' *Communication Through
Coherence* (CTC) hypothesis.

## What you'll learn

In this exercise you will learn

## What to do

To complete this exercise, you should:

1. Start from your previous uncoupled PING pair brn file.
2. Add excitatory synapses from each excitatory module to the inhibitory
module of the other PING. These should be sparse (1% density), have reasonably
strong weights (from 0 to 12) and a moderate delay (5ms).
3. Run the network and add a small current (~5pA) to both excitatory
populations.

If everything went well, you should see both groups oscillating, but unlike
the previous case they should be coordinated and at the same frequency -- we
say they are *synchronized*.


[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[//]: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
# Ex4: Multistable network

## Introduction

Oscillators are not the only interesting neural microcircuit we find in the
cortex. Equally ubiquitous is the phenomenon of multistability -- a set of
excitatory groups that compete against each other in a winner-take-all
fashion. In this exercise, you will implement and run a simple bistable
network (i.e. a multistable set-up with two competing nodes).

## What you'll learn

In this exercise you'll create more sophisticated connectivity patterns and
make use of more BrainStudio's built-in functions. You will also learn about
the structure of brn files and modify one to change the fine details of the
simulation. The specs will be less specific this time so you'll have to find
your way to observe the phenomenon you're looking for.

## What to do

To complete this exercise, you should:

1. Start a new brn file from scratch (you can use the shortcut `Ctrl+N`). Add
two excitatory populations of 400 neurons each and an inhibitory population of
200 neurons, with the same model and parameters as in the previous exercises.
2. Add dense inhibitory connections from the inhibitory module to the whole
network.
3. Add strong, delayed recurrent connections within each excitatory module.
4. Run the simulation and experiment with the stimulation. What phenomena do
you observe? Try changing the intra- and inter-module connectivity settings.
    * *Hint*:


Under some conditions, you can also see *theta-modulated gamma* oscillations
(or theta envelopes), i.e. a base oscillation with gamma frequency and an
amplitude determined by a slower theta frequency.

And that's it! You should be a BrainStudio pro by now. In the next set of
exercises we will cover how to create custom nodes and edges and more advanced
features.
