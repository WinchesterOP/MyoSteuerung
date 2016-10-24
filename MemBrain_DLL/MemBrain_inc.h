#ifndef _MEMBRAIN_INC_H_
#define _MEMBRAIN_INC_H_

/*
This section provides legal information about licenses for software which is used by the MemBrain DLL.

KissFFT
For FFT calculations the MemBrain DLL uses the library 'KissFFT'.
License: BSD License
License information for KissFFT library and related source code ---------------------------- START --------------------------------
Copyright (c) 2003-2010, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions 
are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
	  in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without 
	  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, 
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
License information for KissFFT library and related source code ---------------------------- END ----------------------------------
*/


/*
This is the header file that needs to be included in C/C++ software projects
that shall make use of the MemBrain DLL.

The MemBrain Dll consists of the following files:

MemBrainDll.dll
MemBrainDll.lib
MemBrain_inc.h (This file)

The header provides all interfaces available to the Dll and has to be included
in all C/C++ source files of your software project that make calls to the Dll.
The MemBrainDll.lib file has to be added to the linker options of your software
project.
The MemBrainDll.dll has to be copied either to the directory where your application
resides or to the WINDOWS\SYSTEM32 directory of your Windows installation.


The following section gives a brief explanation of what the features of the
Dll are and how it is used.

1.) Features of the Dll

The MemBrain Dll allows your application to open neural network files created
with MemBrain (standard file extension is *.mbn but does not have to be).
It provides access to the input data of the net (input neuron activations),
allows to perform any number of think steps (i.e. simulation steps of the
network) and to read back the result data from the output neurons (either
activations or output signals whatever is desired).

Since version 03.00.00.00 of the Dll also the complete set functions necessary for
training ('teaching') your neural nets and handling MemBrain lesson files is
available on the DLL level.
By this an application can now also re-train neural nets when new data becomes
available that is not yet handled well by the nets.

By default the Dll operates with one neural net at a time but it provides
interfaces to open an arbitrary number of additional network files and switch
between the opened nets during run time.

There is also the possibility to save a net back to file, either overwriting the
original file or with a new file name. The whole neural net including its current
internal state is saved, i.e. all activation spikes along the links of the net,
all activations etc. are saved together with the net as MemBrain does, too.

The Dll supports normalization of input and output neuron activation values as
MemBrain does. Thus, if an input or output neuron in MemBrain is specified to make
use of a user defined normalization range it will also do in the Dll i.e. the neuron
will accept/report activation values in the user defined range and automatically
convert them to/from its internal activation value range according to its activation
function.

Also the Dll supports execution of executables (e.g. batch files) when a neuron
fires. However, the file that is executed upon a neuron being firing has to be
configured in MemBrain at creation time of the net. I.e. the DLL does not provide
a possibility to re-configure this setting for any of the neurons.

The Dll does not support MemBrain's Weblink. You can open nets with the Dll that
contain public or extern neurons but they will be treated by the Dll as if they
were private neurons of the net. This is the same behaviour as when the Weblink
is kept deactivated in MemBrain.
Also the Dll does not provide support for the MemBrain scripting language. I.e.
you can not execute MemBrain scripts by use of the Dll.



2.) Using the Dll

--- General ---
---------------
If a Dll interface function returns a value of the type EMBRetVal then you should
always check that the returned value is MEMBRAIN_OK which indicates success.
A value of MEMBRAIN_ERR is returned when a function fails, e.g. if you provide
invalid parameters to the function.

Most interface functions of the Dll operate on the so called 'active net'. By
default the Dll administers one single net unless you add further nets by using
the interface _MB_AddNet(). If you only operate on one net at a time then the
default net always is the active one and you don't have to care about selecting
the active net before using any interface function of the Dll.

You can always select the currently active net by using _MB_SelectNet() and
providing the corresponding net index. Indices always start at 0, so the call
_MB_SelectNet(0) selects the first net (which is always available by default
already), _MB_SelectNet(1) selects the net that was created using the first call
to _MB_AddNet() and so on.

The index of the currently active (selected) net can be retrieved at any time
using _MB_GetSelectedNet().

To delete a net use _MB_DeleteNet() with the corresponding net index.
Note that this will cause all nets with higher indices than the deleted one to
have a decremented index from then on. E.g. if you delete the net with the index 2
then the net with the former index 3 will be accessed by index 2 after the call.
If you delete the active net then the net previous to the active one will get active
automatically.

Note that you can never delete the net with the index 0. This is the default net
which can not be deleted!

With the interface function _MB_GetNetCount() you can always check how many nets exist
in the Dll. _MB_GetNetCount() returns 1 by default and will never return a value
lower than 1.

Similarly, the Dll by default administers one lesson which also automatically is
the active lesson. However, further lessons can be added or removed
(function _MB_SetLessonCount()) and the active lesson can be selected by the function
_MB_SelectLesson() or retrieved by the function _MB_GetSelectedLesson().

The number of administered lessons can always be retrieved using _MB_GetLessonCount().


When unloading the Dll (e.g. your application exits) you don't have to care about
deleting the created nets or lessons. This is done by the Dll automatically.


--- Retrieving the version information string ---
-------------------------------------------------
Use _MB_GetVersionInfo() to get a string with the version information of the Dll.
This is of informational use only, i.e. you don't need this interface function to
operate the Dll.
You can provide a maximum string len to be copied to the provided character buffer.
Note that this length excludes the terminating '\0' that is additionally copied
to the end of the string, i.e. if you want to get a maximum string len of 40 you
have to provide a character array (C-string) with 41 bytes.
The returned string is something like the following and will never exceed 40 bytes
length:
"MemBrain Dll V03.00.00.00 (13 Nov 2007)"



--- Loading the active net from file ---
----------------------------------------
You can load the currently active net of the Dll from file using the interface
_MB_LoadNet().
The function deletes all previous contents of the active net before loading it.


--- Resetting the active net ---
-------------------------
Use _MB_ResetNet() to reset the activations of all neurons in the net and the
activation spikes along all links in the net to a value of 0.
Note: This is only useful if your net contains loopback links or links with
a length > 1. In this case this functionality provides a way to initialize
your net to a known internal state.


--- Saving the active net to file ---
-------------------------------------
Use _MB_SaveNet() or _MB_SaveNetAs() to save the active net to file.
A net can change its internal state when Think Steps are performed on the net.
E.g. if the net contains loopback links or links with a length > 1 it is
important to save the net together with all its internal states to be able to
continue operation at exactly the same point when loading the net from file
again at a later time.
Also, certainly your net can be changed through teaching which is supported by
the Dll since version 03.00.00.00, too.


--- Accessing the input neurons of the active net ---
-----------------------------------------------------
To apply an activation value to an input neuron use the Dll interface
_MB_ApplyInputAct with the 0 based index of the input neuron you want to access
and the activation value you want to apply.
All neurons of the input layer of the active net will be enumerated with indices
starting from 0. The numer of available input neurons in the active net can be
retrieved by the Dll interface _MB_GetInputCount().
Enumeration of the input neurons takes places according to their position in the
net row-wise from left to right and from top to bottom. It is the same algorithm
as used in MemBrain if you press the button <Names From Net> in MemBrain's Lesson
Editor.
If you are not sure which input neuron enumerates to which index you can use the
Dll interface _MB_GetInputName() which returns the name of an input neuron according
to the given index. So you can set up your own access table that links neuron names
with corresponding indices if you like to.

There is also a possibility to read back the activation value of an input neuron:
Use the interface _MB_GetInputAct() together with the corresponding input neuron
index for this purpose.
Note that the activation value of an input neuron can change when you perform Think
Steps: If the Activation Sustain Factor of the input neuron is < 1 then the activation
of the neuron will decay starting with the second think step after having applied the
activation. See the MemBrain HTML help for more information on MemBrain's neuron model.


--- Perform Think Steps with the active net ---
-----------------------------------------------
Call the interface _MB_ThinkStep() to perform one Think Step (Simulation Step) of the
active net.
The terminology 'One Think Step' means that every neuron in the net performs one
simulation step and propagates its output signal to the connected other neurons.
If you operate a pure Feed-Forward net with link length values of all 1 then the
following will be the appropriate procedure:
1.) Apply activation values to the input neurons of the net (see above).
2.) Perform one Think Step (call _MB_ThinkStep() once)
3.) Read the results from the output neurons of the net (see below).

If you are interested in more details about Think Steps, especially what the order of
calculations in the net is with respect to layers then please refer to the MemBrain
HTML help.


--- Accessing the output neurons of the active net ---
------------------------------------------------------
Output neurons are enumerated and accessed in the same manner as the input neurons of
the net (see above). Use the interface functions
_MB_GetOutputCount()
_MB_GetOutputName()
_MB_GetOutputAct()
_MB_GetOutputOut()
together with the corresponding output neuron index.
Note that output and input neurons are enumerated separately, so valid indices for
output neurons are always between 0 and _MB_GetOutputCount() - 1.

As you can see there is a function _MB_GetOutputAct() to get the activation of an output
neuron and also a function _MB_GetOutputOut() to get the output value of an output
neuron.
Normally the activation is what you will be interested in as this is the value that has
been trained in MemBrain and also this value is normalized if the neuron uses normalization.
However, sometimes it may be desired to get the output value of an output neuron which
may differ from the activation:
1.) The output value does not use the normalization settings.
2.) The output value can be binary (0 or 1) although the activation function is not
(setting 'Output Fire Level' = 1 in MemBrain).
3.) The output value depends on the Lower and Upper Fire Thresholds of the neuron.

For more details on this see the MemBrain HTML help, section 'Neurons in MemBrain - Neuron
Model and Operation'.


--- Identifying the winner output neuron of the net ---
-------------------------------------------------------
The current output winner neuron can be identified using the interface _MB_GetOutputWinnerNeuron().
The function returns the zero based index of the winner neuron into the output neuron's array.
The winner neuron is the neuron with the highest normalized activation value.
If there is no winner neuron (i.e. no Think Step has been performed so far) then the return
value is -1.


--- Identifying the valid activation range of a neuron ---
-------------------------------------------------------
Through the Dll you can also determine the valid activation range for a neuron. Use the functions
_MB_GetInputActRange resp. _MB_GetOutputActRange to do so.
This takes into account if the neuron uses normalization or not: If the neuron uses normalization
then the functions return the normalization limits as adjusted in MemBrain for the corresponding neuron.
If the neuron does not use normalization the functions return the normalized activation range which
depends on the chosen activation function of the neuron.



--- New interface functions since version 03.00.00.00 ---
-------------------------------------------------------
The Dll has been expanded by a lot of interface functions since version 03.00.00.00.
These new functions allow access to lessons and allow to (re-)train your neural nets from out your own
applications without using the MemBrain main application.
Also you can build your own lessons, save them and/or export them to csv.

Most of the interface functions are self explanatory and operate very similar to the corresponding
MemBrain scripting commands.

The main difference between the scripting commands and the Dll functions is that all indices that
select items like lessons or patterns are 0-based in contrary to the scripting language where these
identifiers start at 1.

In order to train your neural nets with the Dll you first have to load a teacher file that contains the
teachers you want to use for the training. The Dll does not provide interface functions to configure
teachers, you have to do this in MemBrain:
In MemBrain, open the Teacher Manager and configure the teachers you want to use in the Dll. Assign meaningful
names to the teachers because you will select teachers through the Dll by specifying their names!

In the Teacher Manager use the option 'Export Teacher File for use with DLL' to export the file 'Teachers.mbt'
to some location on your hard disk. Copy the file to the working directory of your application.

When the MemBrain Dll starts up (is loaded by your program) it tries to open this file automatically. If this
should not work (depends on system) or if you want to load another teacher file then you have to call the interface
function _MB_LoadTeacherFile() and provide the full path and name of the teacher file to this function.

You can retrieve the number of loaded teachers via the function _MB_GetTeacherCount(). Also you can get the name
of every loaded teacher with the function _MB_GetTeacherName(). You will have to provide the 0-based index of the
teacher you want to request the name of.

Lessons are loaded or imported through the functions _MB_LoadLesson or _MB_ImportLessonRaw for example. You can
choose how many lessons are administered at a time (_MB_SetLessonCount()) and also select the currently active
lesson by calling _MB_SelectLesson().

There is a lot of other useful functions just go through the list and check out what's there...

To finally train your net call the function _MB_TeachStep(). Note that the return value of this function is
special, the meanings of the possible return values are described in the enumeration EMBTeachResult.
Every call to _MB_TeachStep() will perform one teach step according to the teacher settings. I.e. if the
teacher is condigured to a 'Repetitions per Lesson' value of 1 and to a 'Repetitions per Pattern' value of 1
then the function _MB_TeachStep() goes through the lesson one time and performs a single teach step on every
Pattern in the Lesson.

You can retrieve the last known Net Error at any time using _MB_GetLastNetError().

Finally, you can enable Lesson rescording just like in the scripting language or in the Lesson Editor itself
and through this record net calculation results to lessons for validation for example.


If you have any questions beyond this brief explanations then you can also try having a closer look at the
scripting language documentation in the MemBrain Help file since most interface functions of the Dll are pretty
similar to the corresponding scripting commands (remember the difference between 0-based and 1-based handling!).



--- Questions and Feedback ---
------------------------------
If you have any questions or feedback to MemBrain or the MemBrain Dll or if you experience
problems with either software, don't hesitate to contact me at

http://www.membrain-nn.de

or via mail:

thomas.jetter@membrain-nn.de




--- Disclaimer ---
------------------
The MemBrain Dll is subject to licensing according to the terms indicated in the license agreement
that is displayed when installing the MemBrain software.

You can also download and view this license agreement on the MemBrain homepage (http://www.membrain-nn.de).

The MemBrain program and all associated supplementary software including the MemBrain DLL
are distributed "as is". No warranty of any kind is expressed or implied.
You use it at your own risk. Thomas Jetter will not be liable for data loss, damages and
loss of profits or properties or any other kind of damages or loss caused by use or misuse
of this software.

If you incorporate the MemBrain Dll into your own software you always have to include
this disclaimer section.

Copyright 2003 - 2012 Thomas Jetter
*/




/// Allows use of plain C-Compilers, too
#ifdef __cplusplus
extern "C" {
#endif


/// Definition of the MemBrain integer as 32 bit integer
#ifdef USED_BY_ORIGIN
    typedef long _MB_INT;
#else
    typedef long int _MB_INT;
#endif

#ifndef BOOL
    typedef int    BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


/// Possible return values of most MemBrain DLL interfaces
typedef enum
{
    MEMBRAIN_ERR = -1,      ///< An error occured
    MEMBRAIN_OK = 0,        ///< Call successful

} EMBRetVal;


/// Possible teach result return values
typedef enum
{
    MB_TR_OK,								///< Teach step OK, teacher not finished
    MB_TR_TARGET_NET_ERROR_REACHED,			///< The target error has been reached. Teacher finished.
    MB_TR_MAX_NEURONS_ADDED,				///< The maximum number of neurons has been added by the teacher. Teacher finished.
    MB_TR_TEACH_ABORTED,					///< The teacher has been aborted by another operation. Not used up to now
    MB_TR_NOT_IN_SYNC,						///< The net is not in sync with the lesson.
    MB_TR_WRONG_ACT_FUNCTION,				///< The net contains at least one neuron that has an activation function which is incompatible with the teacher
    MB_TR_OUT_OF_LESSON_RANGE,				///< The teacher operates outside the current lesson range. Not used up to now.
    MB_TR_ANALYSIS_ERROR,					///< Teaching is not possible because of architectural errors in net.
    MB_TR_LESSON_EMPTY,						///< Teaching is not possible because the currently active lesson is empty.
    MB_TR_NET_ERR_LESSON_EMPTY,             ///< Teaching is not possible because the currently active net error lesson is empty
    MB_TR_NET_ERR_NOT_IN_SYNC               ///< The net is not in sync with the net error lesson.

} EMBTeachResult;

/// possible activation functions
typedef enum
{
    MB_AF_LOGISTIC         = 0,
    MB_AF_IDENTICAL        = 1,
    MB_AF_IDENTICAL_0_1    = 2,
    MB_AF_TAN_H            = 3,
    MB_AF_BINARY           = 4,
    MB_AF_MIN_EUCLID_DIST  = 5

} EMBActFunc;

// possible input functions
typedef enum
{
    MB_IF_SUM = 0,
    MB_IF_MUL = 1,

} EMBInputFunc;


/// possible output ranges
typedef enum 
{
    MB_OFL_1     = 0,	    // 0 or 1
    MB_OFL_ACT   = 1  		// as activation function calculates

} EMBOutputFireLevel;



#pragma pack(1)
/// Parameter structure to describe neuron properties
typedef struct
{
    double act;
    _MB_INT inputFunc;                      // see EMBInputFunc
    _MB_INT actFunc;                        // see EMBActFunc
    double actThres;
    BOOL lockActThres;
    double actSustain;                      // 0..1
    _MB_INT outputFireLevel;                // see EMBOutputFireLevel
    _MB_INT outputRecovTime;                // 1..100000
    double fireThresLow;                    // fireThresHi must be >= fireThresLo
    double fireThresHi;                     // fireThresHi must be >= fireThresLo
    BOOL useNormalization;
    double normRangeLow;
    double normRangeHigh;
    BOOL useActIgnoreVal;
    double actIgnoreVal;
    double expLogistic;
    double parmTanHyp;
    BOOL allowTeacherOutputConnect;
    BOOL displayName;
    BOOL displayAct;
    BOOL isPixel;
    _MB_INT width;                          // 15 - 30000

} SMBNeuronProp;
#pragma pack()

#pragma pack(1)
/// Parameter structure to describe link properties
typedef struct
{
    double weight;
    BOOL lockWeight;
    _MB_INT length;             // 1..10000
    BOOL displayWeight;

} SMBLinkProp;
#pragma pack()


    /// Set by Visual C++ IDE
#ifndef _USRDLL
#define _USRDLL 0
    // General DLL declaration macro
#ifdef _NO_STDCALL
    #ifdef USED_BY_ORIGIN
        #define MB_DLL_RETURN(retType) retType        
    #else
        #define MB_DLL_RETURN(retType) retType __declspec(dllimport)   
    #endif
#else
    #ifdef USED_BY_ORIGIN
        #define MB_DLL_RETURN(retType) retType __stdcall
    #else
        #define MB_DLL_RETURN(retType) retType __declspec(dllimport) __stdcall
    #endif
#endif
#else
    #ifdef _NO_STDCALL
    // General DLL declaration macro
        #define MB_DLL_RETURN(retType) retType __declspec(dllexport)
    #else
        #define MB_DLL_RETURN(retType) retType __declspec(dllexport) __stdcall
    #endif
#endif


/// Get the version information string of the DLL.
/// Specify maximum length of string to be copied (excluding the terminating '\0'.
/// A terminating '\0' will be attached in every case.
MB_DLL_RETURN(EMBRetVal) _MB_GetVersionInfo(char* pInfo, _MB_INT maxLen);

/// Add a new neural net to the DLL's internal array of neural nets.
MB_DLL_RETURN(void) _MB_AddNet(void);

/// Get number of currently available neural nets in the DLL's array
MB_DLL_RETURN(_MB_INT) _MB_GetNetCount(void);

/// Get index of the currently selected net
MB_DLL_RETURN(_MB_INT) _MB_GetSelectedNet(void);

/// Delete the neural net at index <idx>.
MB_DLL_RETURN(EMBRetVal) _MB_DeleteNet(_MB_INT idx);

/// Select one of the available nets as the currently active one.
MB_DLL_RETURN(EMBRetVal) _MB_SelectNet(_MB_INT idx);

/// Load the currently active neural net from the given *.mbn file (including path)
MB_DLL_RETURN(EMBRetVal) _MB_LoadNet(const char* pFileName);

/// Save a MemBrain neural net to the given *.mbn file (including path)
MB_DLL_RETURN(EMBRetVal) _MB_SaveNetAs(const char* pFileName);

/// Save an already loaded MemBrain neural net (overwrite original file)
MB_DLL_RETURN(EMBRetVal) _MB_SaveNet(void);

/// Reset the net. All activations and link spikes are set to 0.
MB_DLL_RETURN(void) _MB_ResetNet(void);

/// Get number of input neurons in the net
MB_DLL_RETURN(_MB_INT) _MB_GetInputCount(void);

/// Get number of output neurons in the net
MB_DLL_RETURN(_MB_INT) _MB_GetOutputCount(void);

/// Get name of input neuron at index <idx>. Specify maximum length
/// of string to be copied (excluding the terminating '\0'.
/// A terminating '\0' will be attached in every case.
MB_DLL_RETURN(EMBRetVal) _MB_GetInputName(_MB_INT idx, char* pName, _MB_INT maxLen);

/// Get name of output neuron at index <idx>. Specify maximum length
/// of string to be copied (excluding the terminating '\0'.
/// A terminating '\0' will be attached in every case.
MB_DLL_RETURN(EMBRetVal) _MB_GetOutputName(_MB_INT idx, char* pName, _MB_INT maxLen);

/// Apply an activation value to the input neuron at index <idx>.
MB_DLL_RETURN(EMBRetVal) _MB_ApplyInputAct(_MB_INT idx, double act);

/// Get the activation value of the input neuron at index <idx>.
MB_DLL_RETURN(EMBRetVal) _MB_GetInputAct(_MB_INT idx, double* pAct);

/// Perform one think step of the net
MB_DLL_RETURN(void) _MB_ThinkStep(void);

/// Get the activation value of the output neuron at index <idx>.
MB_DLL_RETURN(EMBRetVal) _MB_GetOutputAct(_MB_INT idx, double* pAct);

/// Get the output value of the output neuron at index <idx>.
MB_DLL_RETURN(EMBRetVal) _MB_GetOutputOut(_MB_INT idx, double* pOutVal);

/// Get index of the last output winner neuron of the net. Return -1 if unknown. Else
/// return the output neuron index of the winner neuron.
MB_DLL_RETURN(_MB_INT) _MB_GetOutputWinnerNeuron(void);

/// Get the activation range of the input neuron at index <idx>.
/// Note: For neurons with AF_IDENTICAL the normalization limits are returned if the neuron uses
/// normalization. Still AF_IDENTICAL can handel activations outside this range. All other
/// actuvatuion functions are limiting the activation values.
MB_DLL_RETURN(EMBRetVal) _MB_GetInputActRange(_MB_INT idx, double* pActMin,
    double* pActMax);

/// Get the activation range of the output neuron at index <idx>.
/// Note: For neurons with AF_IDENTICAL the normalization limits are returned if the neuron uses
/// normalization. Still AF_IDENTICAL can handel activations outside this range. All other
/// actuvatuion functions are limiting the activation values.
MB_DLL_RETURN(EMBRetVal) _MB_GetOutputActRange(_MB_INT idx, double* pActMin,
    double* pActMax);

/// Set the activation range of the input neuron at index <idx>.
MB_DLL_RETURN(EMBRetVal) _MB_SetInputActRange(_MB_INT idx, double actMin,
    double actMax);

/// Set the activation range of the output neuron at index <idx>.
MB_DLL_RETURN(EMBRetVal) _MB_SetOutputActRange(_MB_INT idx, double actMin,
    double actMax);

/// Set the separators used for CSV file parsing (list separator and decimal separator).
MB_DLL_RETURN(EMBRetVal) _MB_SetCsvFileSeparators(const char* pListSep, const char* pDecSep);

/// Get the separators used for CSV file parsing (list separator and decimal separator). The buffers must be 2 bytes long each at minimum.
MB_DLL_RETURN(EMBRetVal) _MB_GetCsvFileSeparators(char* pListSep, char* pDecSep);




////--------------------- For lesson handling and teaching --------------------
/// Load a lesson to be the currently active
MB_DLL_RETURN(EMBRetVal) _MB_LoadLesson(const char* filePath);


/// Import the currently active lesson from csv
MB_DLL_RETURN(EMBRetVal) _MB_ImportLesson(const char* filePath);


/// Import the currently active lesson from raw csv
MB_DLL_RETURN(EMBRetVal) _MB_ImportLessonRaw(const char* filePath);


/// Import the currently active lesson inputs from raw csv
MB_DLL_RETURN(EMBRetVal) _MB_ImportLessonInputsRaw(const char* filePath);


/// Import the currently active lesson outputs from raw csv
MB_DLL_RETURN(EMBRetVal) _MB_ImportLessonOutputsRaw(const char* filePath);


/// Save the currently active lesson to its current file name
MB_DLL_RETURN(EMBRetVal) _MB_SaveLesson(void);


/// Save the currently active lesson to the given file name
MB_DLL_RETURN(EMBRetVal) _MB_SaveLessonAs(const char*  filePath);


/// Export the currently active lesson to csv. Specify maxCols with 0 to export with the full width of all columns
MB_DLL_RETURN(EMBRetVal) _MB_ExportLesson(const char* filePath, _MB_INT maxCols);

/// Export the currently active lesson to raw csv. Specify maxCols with 0 to export with the full width of all columns
MB_DLL_RETURN(EMBRetVal) _MB_ExportLessonRaw(const char* filePath,	_MB_INT maxCols);

/// Export the inputs of the currently active lesson to raw csv. Specify maxCols with 0 to export with the full width of all columns
MB_DLL_RETURN(EMBRetVal) _MB_ExportLessonInputsRaw(const char* filePath, _MB_INT maxCols);

/// Export the outputs of the currently active lesson to raw csv. Specify maxCols with 0 to export with the full width of all columns
MB_DLL_RETURN(EMBRetVal) _MB_ExportLessonOutputsRaw(const char* filePath, _MB_INT maxCols);


/// Set the number of inputs of the currently administered lesson
MB_DLL_RETURN(EMBRetVal) _MB_SetLessonInputCount(_MB_INT count);


/// Get the number of inputs of the currently administered lesson
MB_DLL_RETURN(_MB_INT) _MB_GetLessonInputCount(void);


/// Set the number of outputs of the currently administered lesson
MB_DLL_RETURN(EMBRetVal) _MB_SetLessonOutputCount(_MB_INT count);


/// Get the number of outputs of the currently administered lesson
MB_DLL_RETURN(_MB_INT) _MB_GetLessonOutputCount(void);


/// Set the input name at index <idx> of the currently active lesson
MB_DLL_RETURN(EMBRetVal) _MB_SetLessonInputName(_MB_INT idx, const char* pName);


/// Get the input name at index <idx> of the currently active lesson
MB_DLL_RETURN(EMBRetVal) _MB_GetLessonInputName(_MB_INT idx, char* pName, int maxLen);


/// Set the output name at index <idx> of the currently active lesson
MB_DLL_RETURN(EMBRetVal) _MB_SetLessonOutputName(_MB_INT idx, const char* pName);


/// Get the output name at index <idx> of the currently active lesson
MB_DLL_RETURN(EMBRetVal) _MB_GetLessonOutputName(_MB_INT idx, char* pName, int maxLen);


/// Set the input value at index <idx> of the current pattern
MB_DLL_RETURN(EMBRetVal) _MB_SetPatternInput(_MB_INT idx, double value);


/// Get the input value at index <idx> of the current pattern
MB_DLL_RETURN(EMBRetVal) _MB_GetPatternInput(_MB_INT idx, double* pValue);


/// Set the output value at index <idx> of the current pattern
MB_DLL_RETURN(EMBRetVal) _MB_SetPatternOutput(_MB_INT idx, double value);


/// Get the output value at index <idx> of the current pattern
MB_DLL_RETURN(EMBRetVal) _MB_GetPatternOutput(_MB_INT idx, double* pValue);


/// Select the currently active pattern of the currently active lesson
MB_DLL_RETURN(EMBRetVal) _MB_SelectPattern(_MB_INT idx);


/// Get the currently selected pattern index of the currently active lesson
MB_DLL_RETURN(_MB_INT) _MB_GetSelectedPattern(void);


/// Delete the currently active pattern of the currently active lesson
MB_DLL_RETURN(EMBRetVal) _MB_DeletePattern(void);


/// Add a pattern to the end of the active lesson
MB_DLL_RETURN(void) _MB_AddPattern(void);


/// Get the number of patterns in the active lesson
MB_DLL_RETURN(_MB_INT) _MB_GetLessonSize(void);


/// Enable/Disable the output data section of the active lesson
MB_DLL_RETURN(EMBRetVal) _MB_EnableLessonOutData(_MB_INT outDataEnabled);


/// Transfer I/O names names and count from the currently active net to
/// the currently active lesson
MB_DLL_RETURN(EMBRetVal) _MB_NamesFromNet(void);


/// Transfer I/O names names from the currently active lesson to
/// the currently active net
MB_DLL_RETURN(EMBRetVal) _MB_NamesToNet(void);


/// Set the number of currently administered lessons
MB_DLL_RETURN(EMBRetVal) _MB_SetLessonCount(_MB_INT count);


/// Get the number of currently administered lessons
MB_DLL_RETURN(_MB_INT) _MB_GetLessonCount(void);


/// Select the active lesson
MB_DLL_RETURN(EMBRetVal) _MB_SelectLesson(_MB_INT idx);


/// Get the index of the currently active lesson
MB_DLL_RETURN(_MB_INT) _MB_GetSelectedLesson(void);


// Select the net error lesson
MB_DLL_RETURN(EMBRetVal) _MB_SelectNetErrLesson(_MB_INT idx);


/// Get the index of the currently active net error lesson
MB_DLL_RETURN(_MB_INT) _MB_GetSelectedNetErrLesson(void);


/// Apply the currently active pattern to the inputs of the currently active net
MB_DLL_RETURN(EMBRetVal) _MB_ApplyPattern(void);


/// Select the current recording type: 0 = Activation, 1 = Output
MB_DLL_RETURN(EMBRetVal) _MB_SetRecordingType(_MB_INT type);


/// Start recording data to a lesson: Specify lesson index to record to and step count
MB_DLL_RETURN(EMBRetVal) _MB_StartRecording(_MB_INT lessonIdx, _MB_INT stepCount);


/// Stop recording data to lesson
MB_DLL_RETURN(void) _MB_StopRecording(void);


/// Think on all patters of the currently active lesson
MB_DLL_RETURN(EMBRetVal) _MB_ThinkLesson(void);


/// Load a teacher file
MB_DLL_RETURN(EMBRetVal) _MB_LoadTeacherFile(const char* filePath);


/// Select the active teacher by name
MB_DLL_RETURN(EMBRetVal) _MB_SelectTeacher(const char* name);


/// Perform one teach step (lesson run). Return result according to Teacher.h (TR_....)
MB_DLL_RETURN(EMBTeachResult) _MB_TeachStep(void);


/// Conclude the current teach run. Should be called after every teach process completion.
MB_DLL_RETURN(void) _MB_StopTeaching(void);


/// Get the number of available teachers
MB_DLL_RETURN(_MB_INT) _MB_GetTeacherCount(void);


/// Get the name of the teacher at index <idx> Specify maximum length
/// of string to be copied (excluding the terminating '\0'.
/// A terminating '\0' will be attached in every case.
MB_DLL_RETURN(EMBRetVal) _MB_GetTeacherName(_MB_INT idx, char* pName, _MB_INT maxLen);

/// Randomize the currently active net
MB_DLL_RETURN(void) _MB_RandomizeNet(void);


/// Get the last known error of the currently active net
MB_DLL_RETURN(double) _MB_GetLastNetError(void);

/// Create an FFT lesson from the currently active lesson
MB_DLL_RETURN(EMBRetVal) _MB_CreateFftLesson(BOOL isComplex, BOOL inputsAreColumns, _MB_INT minFreqIdx, _MB_INT maxFreqPoints);

/// Get frequency that corresponds to a frequency index in an FFT
MB_DLL_RETURN(double) _MB_GetFftFrequency(_MB_INT freqIdx, double overallSampleTime);

/// Create a new lesson with averaged inputs from the current lesson
MB_DLL_RETURN(EMBRetVal) _MB_CreateAverageLesson(_MB_INT newInputDimension);


////--------------------- For password handling --------------------
/// Set the password for the currently loaded net. Valid number of characters = [1 .. 32]
MB_DLL_RETURN(EMBRetVal) _MB_SetNetFilePwd(const char* pwd);

/// Remove the password for the currently loaded net
MB_DLL_RETURN(void) _MB_RemoveNetFilePwd();

/// Set the password for the currently selected lesson. Valid number of characters = [1 .. 32]
MB_DLL_RETURN(EMBRetVal) _MB_SetLessonFilePwd(const char* pwd);

/// Remove the password for the currently selected lesson
MB_DLL_RETURN(void) _MB_RemoveLessonFilePwd();

/// Set the password used for opening files. Valid number of characters = [1 .. 32]
MB_DLL_RETURN(EMBRetVal) _MB_SetFileOpenPwd(const char* pwd);

/// Remove the password used for opening files
MB_DLL_RETURN(void) _MB_RemoveFileOpenPwd();



/// ---------------- Functions for editing neural nets down from here---------------------------

/// If you have any questions beyond the brief function descriptions given below then you can also try having a 
/// closer look at the scripting language documentation in the MemBrain Help file since most interface functions 
/// of the Dll are pretty similar to the corresponding scripting commands (remember the difference between 
/// 0-based and 1-based handling!).


/// Clear selection
MB_DLL_RETURN(void) _MB_ClearSelection();

/// Select an input neuron
MB_DLL_RETURN(EMBRetVal) _MB_SelectInput(_MB_INT InNeuronIdx, BOOL addToSelection);

/// Select all input neurons
MB_DLL_RETURN(void) _MB_SelectAllInputs(BOOL addToSelection);

/// Select an output neuron
MB_DLL_RETURN(EMBRetVal) _MB_SelectOutput(_MB_INT outNeuronIdx, BOOL addToSelection);

/// Select all output neurons
MB_DLL_RETURN(void) _MB_SelectAllOutputs(BOOL addToSelection);

/// Get the number of hidden layers in the net
MB_DLL_RETURN(_MB_INT) _MB_GetHiddenLayerCount();            

/// Get the number of neurons in a given hidden layer
MB_DLL_RETURN(_MB_INT) _MB_GetHiddenCount(_MB_INT hidLayerIdx);  

/// Get the number of neurons in all hidden layers
MB_DLL_RETURN(_MB_INT) _MB_GetHiddenCountAll();  

/// Get the number of neurons in the context layer
MB_DLL_RETURN(_MB_INT) _MB_GetContextCount();   

/// Get the number of neurons in the unresolved layer
MB_DLL_RETURN(_MB_INT) _MB_GetUnresolvedCount(); 

/// Select a hidden neuron
MB_DLL_RETURN(EMBRetVal) _MB_SelectHidden(_MB_INT hidLayerIdx, _MB_INT hidNeuronIdx, BOOL addToSelection);

/// Select all neurons in a hidden layer
MB_DLL_RETURN(EMBRetVal) _MB_SelectHiddenLayer(_MB_INT hidLayerIdx, BOOL addToSelection);

/// Select all hidden neurons
MB_DLL_RETURN(void) _MB_SelectAllHidden(BOOL addToSelection);

/// Select a context neuron
MB_DLL_RETURN(EMBRetVal) _MB_SelectContext(_MB_INT neuronIdx, BOOL addToSelection);

/// Select all context neurons
MB_DLL_RETURN(void) _MB_SelectAllContexts(BOOL addToSelection);

/// Select an unresolved neuron
MB_DLL_RETURN(EMBRetVal) _MB_SelectUnresolved(_MB_INT neuronIdx, BOOL addToSelection);

/// Select all unresolved neurons
MB_DLL_RETURN(void) _MB_SelectAllUnresolved(BOOL addToSelection);

/// Clear the Extra Selection
MB_DLL_RETURN(void) _MB_ClearExtraSelection();

/// Apply Extra Selection to the current selection
MB_DLL_RETURN(void) _MB_ExtraSelection();

/// Connect FROM Extra Selection
MB_DLL_RETURN(void) _MB_ConnectFromExtra();  

/// Connect TO Extra Selection
MB_DLL_RETURN(void) _MB_ConnectToExtra();  

/// Add an input neuron to the net
MB_DLL_RETURN(void) _MB_AddInput(_MB_INT posX, _MB_INT posY, const char* pName); 

/// Add an output neuron to the net
MB_DLL_RETURN(void) _MB_AddOutput(_MB_INT posX, _MB_INT posY, const char* pName); 

/// Add an unresolved hidden neuron to the net
MB_DLL_RETURN(void) _MB_AddHidden(_MB_INT posX, _MB_INT posY, const char* pName); 


/// Get the properties of the currently selected neuron
MB_DLL_RETURN(EMBRetVal) _MB_GetSelectedNeuronProp(SMBNeuronProp* pProp);

/// Set the properties of the selected neurons
MB_DLL_RETURN(EMBRetVal) _MB_SetSelectedNeuronProp(const SMBNeuronProp* pProp);

/// Delete the selected objects
MB_DLL_RETURN(void) _MB_DeleteSelectedObjects();

/// Prepare for a new net
MB_DLL_RETURN(void) _MB_ClearNet();

/// Set the name of the selected neurons    
MB_DLL_RETURN(void) _MB_SetSelectedNeuronName(const char* pName);

/// Move all selected neurons
MB_DLL_RETURN(void) _MB_MoveSelectedNeurons(_MB_INT dX, _MB_INT dY);

/// Select all links from Extra Selection to Selection
MB_DLL_RETURN(void) _MB_SelectFromExtra();

/// Select all links from Selection to Extra Selection
MB_DLL_RETURN(void) _MB_SelectToExtra();  


/// Get the properties of the selected link
MB_DLL_RETURN(EMBRetVal) _MB_GetSelectedLinkProp(SMBLinkProp* pProp);

/// Set the properties of the selected links
MB_DLL_RETURN(EMBRetVal) _MB_SetSelectedLinkProp(const SMBLinkProp* pProp);

/// Get the position of the selected neuron
MB_DLL_RETURN(EMBRetVal) _MB_GetSelectedNeuronPos(_MB_INT* pX, _MB_INT* pY);          

/// Get the nearest grid point to a given point
MB_DLL_RETURN(EMBRetVal) _MB_GetGridPoint(_MB_INT x, _MB_INT y, _MB_INT* pGridX, _MB_INT* pGridY);

/// Get the adjusted grid width
MB_DLL_RETURN(_MB_INT) _MB_GetGridWidth();      

/// Get a random value between 0 an 1
MB_DLL_RETURN(double) _MB_Random(); 







#ifdef __cplusplus
}
#endif

#endif