// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: utils.h
// Description: Provides simple utility functions for working with resistors
//              and diodes.
// ===========================================================================
#pragma once

double parallel(double a, double b);
double parallel(double a, double b, double c);

double diodeNonlinearity(double voltage);
