/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                                                                           *
 *  pdoct is a Pure Data external for processing audio using GNU Octave      *
 *  scripts.                                                                 *
 *                                                                           *
 *  Copyright (C) 2020 Sebastian Boli Hildebrandt.                           *
 *                                                                           *
 *  This program is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include <stdbool.h>

#ifdef __cplusplus
    extern "C" {
#endif

bool octwrapper_run(const char *funcname, float **input, unsigned ninput, float **output, unsigned noutput, unsigned nsamples, const char* paramid, float* paramval, unsigned nparam);
bool octwrapper_init();

#ifdef __cplusplus
    }
#endif