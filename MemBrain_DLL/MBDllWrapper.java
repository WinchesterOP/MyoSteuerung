package de.membrainminusnn;
/**
This is a Java Wrapper class that allows to access the MemBrain DLL from Java
on Windows platforms using the JNI (Java Native Interface).

For 32 bit java engine:
Copy the files MBDllWrapper.dll and MemBrainDll.dll into the working directory of
your Java application.

For 64 bit java engine:
Copy the files MBDllWrapper64.dll and MemBrainDll64.dll into the working directory of
your Java application.
Comment out the instruction 'System.loadLibrary("MBDllWrapper");' and replace it with 
'System.loadLibrary("MBDllWrapper64")'. 

Then just call the methods of this wrapper class to access the functions of the
MemBrain DLL.

Example:

MBDllWrapper.MBApplyInputAct(0, 0.5);

This applies the activation value '0.5' to the input neuron at index 0 of the currently
selected neural net.

When DLL functions fail during execution for some reason then an error code != 0 is
stored inside the wrapper DLL. It can be retrieved at any time using

MBDllWrapper.GetLastError(); // 0 means no error != 0 means an error has occurred.

Note that this call automatically clears the error code in the wrapper DLL.

You can check the error code after every single call to the wrapper DLL or only
after a sequence of several calls. Once an error occurs it will be stored in the
wrapper DLL until it is either retrieved using the above mentioned function or
overwritten by another error code != 0.

For more information on using the interface functions of the class MBDllWrapper see
the file MemBrain_inc.h which is located in the sub folder \DLL of your MemBrain
installation directory. It describes all MemBrain DLL interface functions in detail
and gives a brief 'How to Use the DLL'.
Most of the interface function of the Java class MBDllWrapper are pretty much identical
to the corresponding DLL interface functions. Only difference is that where a DLL function
returns an error code (either 0, i.e. success or != 0, i.e. error) the java wrapper class
will store the error internally so it can be retrieved as mentioned above.

The reason for this is that Java does not support 'Call by Reference' on simple data types,
so a function can only return one single value of a simple data type to the caller
(if arrays are not used which has been avoided here). Thus the functions return the actual
'target value' instead of the error code and the error code return is handled separately
through the method MBDllWrapper.GetLastError().

Because of the same reason the DLL interface functions _MB_GetInputActRange and
_MB_GetOutputActRange have been split into two separate functions each (MBGetInputActRangeMin
and MBGetInputActRangeMax as well as MBGetOutputActRangeMin and MBGetOutputActRangeMax).
*/




public class MBDllWrapper
{
	/// Possible teach result return values (method MBTeachStep())
	public static int MB_TR_OK = 0; 								///< Teach step OK, teacher not finished
	public static int MB_TR_TARGET_NET_ERROR_REACHED = 1;			///< The target error has been reached. Teacher finished.
	public static int MB_TR_MAX_NEURONS_ADDED = 2;					///< The maximum number of neurons has been added by the teacher. Teacher finished.
	public static int MB_TR_TEACH_ABORTED = 3;						///< The teacher has been aborted by another operation. Not used up to now
	public static int MB_TR_NOT_IN_SYNC = 4;						///< The net is not in sync with the lesson.
	public static int MB_TR_WRONG_ACT_FUNCTION = 5;				///< The net contains at least one neuron that has an activation function which is incompatible with the teacher
	public static int MB_TR_OUT_OF_LESSON_RANGE = 6;				///< The teacher operates outside the current lesson range. Not used up to now.
	public static int MB_TR_ANALYSIS_ERROR = 7;					///< Teaching is not possible because of architectural errors in net.
	public static int MB_TR_LESSON_EMPTY = 8;						///< Teaching is not possible because the currently active lesson is empty.
	public static int MB_TR_NET_ERR_LESSON_EMPTY = 9;				///< Net error lesson is emtpy
	public static int MB_TR_NET_ERR_NOT_IN_SYNC = 10;				///< Net error lesson is not in sync with the net
	
	
	/// Possible input functions
	public static int MB_IF_SUM = 0;
	public static int MB_IF_MUL = 1;
	
	/// Possible activation functions
   	public static int MB_AF_LOGISTIC = 0;
	public static int MB_AF_IDENTICAL = 1;
	public static int MB_AF_IDENTICAL_0_1 = 2;
	public static int MB_AF_TAN_H = 3;
	public static int MB_AF_BINARY = 4;
	public static int MB_AF_MIN_EUCLID_DIST = 5;
    
    /// Possible output fire levels
  	public static int MB_OFL_1 = 0;	    // 0 or 1
	public static int MB_OFL_ACT = 1;  	// as activation function calculates
    
    /// Parameter structure to describe neuron properties
    public static class MBNeuronProp
    {
        public double act;
        public int inputFunc;                      		// see valid constant definitions above
        public int actFunc;                      		// see valid constant definitions above
        public double actThres;
        public boolean lockActThres;
        public double actSustain;                       // 0..1
        public int outputFireLevel;      				// see valid constant definitions above
        public int outputRecovTime;                     // 1..100000
        public double fireThresLow;                     // fireThresHi must be >= fireThresLo
        public double fireThresHi;                      // fireThresHi must be >= fireThresLo
        public boolean useNormalization;
        public double normRangeLow;
        public double normRangeHigh;
        public boolean useActIgnoreVal;
        public double actIgnoreVal;
        public double expLogistic;
        public double parmTanHyp;
        public boolean allowTeacherOutputConnect;
        public boolean displayName;
        public boolean displayAct;
        public boolean isPixel;
        public int width; 
    };
    
    /// Parameter structure to describe link properties
    public static class MBLinkProp
    {
        public double weight;
        public boolean lockWeight;
        public int length;             // 1..10000
        public boolean displayWeight;
    };
	
	static
	{
	    //System.loadLibrary("MBDllWrapper");
	    // For 64 bit java engines use the following line instead:
	    System.loadLibrary("MBDllWrapper64");
	}

	/// Get the last error code (if any, else 0). Call automatically clears error for sure.
	public static native int GetLastError();

	/// Get the version information string of the Wrapper DLL.
	public static native String GetWrapperVersionInfo();

	/// Get the version information string of the DLL.
	public static native String MBGetVersionInfo();

	/// Add a new neural net to the DLL's internal array of neural nets.
	public static native void MBAddNet();

	/// Get number of currently available neural nets in the DLL's array
	public static native int MBGetNetCount();

	/// Get index of the currently selected net
	public static native int MBGetSelectedNet();

	/// Delete the neural net at index <idx>.
	public static native void MBDeleteNet(int idx);

	/// Select one of the available nets as the currently active one.
	public static native void MBSelectNet(int idx);

	/// Load the currently active neural net from the given *.mbn file (including path)
	public static native void MBLoadNet(String pathFile);

	/// Save the currently active neural net to the given *.mbn file (including path)
	public static native void MBSaveNetAs(String pathFile);

	/// Save the currently active neural net (overwrite original file)
	public static native void MBSaveNet();

	/// Reset the net. All activations and link spikes are set to 0.
	public static native void MBResetNet();

	/// Get number of input neurons in the net
	public static native int MBGetInputCount();

	/// Get number of input neurons in the net
	public static native int MBGetOutputCount();

	/// Get name of input neuron at index <idx>.
	public static native String MBGetInputName(int idx);

	/// Get name of output neuron at index <idx>.
	public static native String MBGetOutputName(int idx);

	/// Apply an activation value to the input neuron at index <idx>.
	public static native void MBApplyInputAct(int idx, double act);

	/// Get the activation value of the input neuron at index <idx>.
	public static native double MBGetInputAct(int idx);

	/// Perform one think step of the net
	public static native void MBThinkStep();

	/// Get the activation value of the output neuron at index <idx>.
	public static native double MBGetOutputAct(int idx);

	/// Get the output value of the output neuron at index <idx>.
	public static native double MBGetOutputOut(int idx);

	/// Get index of the last output winner neuron of the net. Return -1 if unknown. Else
	/// return the output neuron index of the winner neuron.
	public static native int MBGetOutputWinnerNeuron();

	/// Get the activation range minimum of the input neuron at index <idx>.
	public static native double MBGetInputActRangeMin(int idx);

	/// Get the activation range maximum of the input neuron at index <idx>.
	public static native double MBGetInputActRangeMax(int idx);

	/// Get the activation range minimum of the output neuron at index <idx>.
	public static native double MBGetOutputActRangeMin(int idx);

	/// Get the activation range maximum of the output neuron at index <idx>.
	public static native double MBGetOutputActRangeMax(int idx);
	
	/// Set the activation range of the input neuron at index <idx>.
	public static native void MBSetInputActRange(int idx, double actMin, double actMax);
	
	/// Set the activation range of the output neuron at index <idx>.
	public static native void MBSetOutputActRange(int idx, double actMin, double actMax);

	/// Get CSV file list separator character.
	public static native String MBGetCsvFileListSeparator();
	
	/// Get CSV file decimal separator character.
	public static native String MBGetCsvFileDecimalSeparator();
	
	/// Set CSV file separators
	public static native void MBSetCsvFileSeparators(String listSep, String decSep);


////--------------------- For lesson handling and teaching --------------------
	/// Load a lesson to be the currently active
	public static native void MBLoadLesson(String pathFile);

	/// Import the currently active lesson from csv
	public static native void MBImportLesson(String pathFile);

	/// Import the currently active lesson from raw csv
	public static native void MBImportLessonRaw(String pathFile);

	/// Import the currently active lesson inputs from raw csv
	public static native void MBImportLessonInputsRaw(String pathFile);

	/// Import the currently active lesson outputs from raw csv
	public static native void MBImportLessonOutputsRaw(String pathFile);

	/// Save the currently active lesson to its current file name
	public static native void MBSaveLesson();

	/// Save the currently active lesson to the given file name
	public static native void MBSaveLessonAs(String pathFile);

	/// Export the currently active lesson to csv. Specify maxCols with 0 to export with the full width of all columns
	public static native void MBExportLesson(String pathFile, int maxCols);

	/// Export the currently active lesson to raw csv. Specify maxCols with 0 to export with the full width of all columns
	public static native void MBExportLessonRaw(String pathFile, int maxCols);

	/// Export the inputs of the currently active lesson to raw csv. Specify maxCols with 0 to export with the full width of all columns
	public static native void MBExportLessonInputsRaw(String pathFile, int maxCols);

	/// Export the outputs of the currently active lesson to raw csv. Specify maxCols with 0 to export with the full width of all columns
	public static native void MBExportLessonOutputsRaw(String pathFile, int maxCols);

	/// Set the number of inputs of the currently administered lesson
	public static native void MBSetLessonInputCount(int count);

	/// Get the number of inputs of the currently administered lesson
	public static native int MBGetLessonInputCount();

	/// Set the number of outputs of the currently administered lesson
	public static native void MBSetLessonOutputCount(int count);

	/// Get the number of outputs of the currently administered lesson
	public static native int MBGetLessonOutputCount();

	/// Set the input name at index <idx> of the currently active lesson
	public static native void MBSetLessonInputName(int idx,  String name);

	/// Get the input name at index <idx> of the currently active lesson
	public static native String MBGetLessonInputName(int idx);

	/// Set the output name at index <idx> of the currently active lesson
	public static native void MBSetLessonOutputName(int idx,  String name);

	/// Get the output name at index <idx> of the currently active lesson
	public static native String MBGetLessonOutputName(int idx);

	/// Set the input value at index <idx> of the current pattern
	public static native void MBSetPatternInput(int idx, double value);

	/// Get the input value at index <idx> of the current pattern
	public static native double MBGetPatternInput(int idx);

	/// Set the output value at index <idx> of the current pattern
	public static native void MBSetPatternOutput(int idx, double value);

	/// Get the output value at index <idx> of the current pattern
	public static native double MBGetPatternOutput(int idx);

	/// Select the currently active pattern of the currently active lesson
	public static native void MBSelectPattern(int idx);

	/// Get the currently selected pattern index of the currently active lesson
	public static native int MBGetSelectedPattern();

	/// Delete the currently active pattern of the currently active lesson
	public static native void MBDeletePattern();

	/// Add a pattern to the end of the active lesson
	public static native void MBAddPattern();

	/// Get the number of patterns in the active lesson
	public static native int MBGetLessonSize();

	/// Enable/Disable the output data section of the active lesson
	public static native void MBEnableLessonOutData(int outDataEnabled);

	/// Transfer I/O names names and count from the currently active net to
	/// the currently active lesson
	public static native void MBNamesFromNet();

	/// Transfer I/O names names from the currently active lesson to
	/// the currently active net
	public static native void MBNamesToNet();

	/// Set the number of currently administered lessons
	public static native void MBSetLessonCount(int count);

	/// Get the number of currently administered lessons
	public static native int MBGetLessonCount();

	/// Select the active lesson
	public static native void MBSelectLesson(int idx);

	/// Get the index of the currently active lesson
	public static native int MBGetSelectedLesson();

	/// Select the active net error lesson (value < 0 selects the net error lesson as to be the training lesson)
	public static native void MBSelectNetErrLesson(int idx);

	/// Get the index of the currently active net error lesson
	public static native int MBGetSelectedNetErrLesson();

	/// Apply the currently active pattern to the inputs of the currently active net
	public static native void MBApplyPattern();

	/// Select the current recording type: 0 = Activation, 1 = Output
	public static native void MBSetRecordingType(int type);

	/// Start recording data to a lesson: Specify lesson index to record to and step count
	public static native void MBStartRecording(int lessonIdx, int stepCount);

	/// Stop recording data to lesson
	public static native void MBStopRecording();

	/// Think on all patters of the currently active lesson
	public static native void MBThinkLesson();

	/// Load a teacher file
	public static native void MBLoadTeacherFile(String pathFile);

	/// Select the active teacher by name
	public static native void MBSelectTeacher(String name);

	/// Perform one teach step (lesson run). Return result according to Teacher.h (TR_....)
	public static native int MBTeachStep();

	/// Conclude the current teach run. Should be called after every teach process completion.
	public static native void MBStopTeaching();

	/// Get the number of available teachers
	public static native int MBGetTeacherCount();

	/// Get the name of the teacher at index <idx> Specify maximum length
	/// of String to be copied (excluding the terminating '\0'.
	/// A terminating '\0' will be attached in every case.
	public static native String MBGetTeacherName(int idx);

	/// Randomize the currently active net
	public static native void MBRandomizeNet();

	/// Get the last known error of the currently active net
	public static native double MBGetLastNetError();
	
	/// Create an FFT lesson from the currently active lesson
	public static native void MBCreateFftLesson(int complex, int inputsAreColumns, int minFreqIdx, int maxFreqPoints);

	/// Get freqeuncy that corresponds to a frequency index in an FFT
	public static native double MBGetFftFrequency(int freqIdx, double overallSampleTime);

	/// Create a new lesson with averaged inputs from the current lesson
	public static native void MBCreateAverageLesson(int newInputDimension);
	
	////--------------------- For password handling --------------------
	/// Set the password for the currently loaded net. Valid number of characters = [1 .. 32]
	public static native void MBSetNetFilePwd(String pwd);
	
	/// Remove the password for the currently loaded net
	public static native void MBRemoveNetFilePwd();
	
	/// Set the password for the currently selected lesson. Valid number of characters = [1 .. 32]
	public static native void MBSetLessonFilePwd(String pwd);
	
	/// Remove the password for the currently selected lesson
	public static native void MBRemoveLessonFilePwd();
	
	/// Set the password used for opening files. Valid number of characters = [1 .. 32]
	public static native void MBSetFileOpenPwd(String pwd);
	
	/// Remove the password used for opening files
	public static native void MBRemoveFileOpenPwd();
	
	
	// -------------------- Functions for editing neural nets down from here ------------------------
	/// Clear selection
	public static native void MBClearSelection();
	
	/// Select an input neuron
	public static native void MBSelectInput(int inNeuronIdx, boolean addToSelection);
	
	/// Select all input neurons
	public static native void MBSelectAllInputs(boolean addToSelection);
	
	/// Select an output neuron
	public static native void MBSelectOutput(int outNeuronIdx, boolean addToSelection);
	
	/// Select all output neurons
	public static native void MBSelectAllOutputs(boolean addToSelection);

	/// Get the number of hidden layers in the net
	public static native int MBGetHiddenLayerCount();

	/// Get the number of neurons in a given hidden layer
	public static native int MBGetHiddenCount(int hidLayerIdx);

	/// Get the number of neurons in all hidden layers
	public static native int MBGetHiddenCountAll();

	/// Get the number of neurons in the context layer
	public static native int MBGetContextCount();

	/// Get the number of neurons in the unresolved layer
	public static native int MBGetUnresolvedCount();

	/// Select a hidden neuron
	public static native void MBSelectHidden(int hidLayerIdx, int hidNeuronIdx, boolean addToSelection);

	/// Select all neurons in a hidden layer
	public static native void MBSelectHiddenLayer(int hidLayerIdx, boolean addToSelection);

	/// Select all hidden neurons
	public static native void MBSelectAllHidden(boolean addToSelection);

	/// Select a context neuron
	public static native void MBSelectContext(int neuronIdx, boolean addToSelection);

	/// Select all context neurons
	public static native void MBSelectAllContexts(boolean addToSelection);

	/// Select an unresolved neuron
	public static native void MBSelectUnresolved(int neuronIdx, boolean addToSelection);

	/// Select all unresolved neurons
	public static native void MBSelectAllUnresolved(boolean addToSelection);

	/// Clear the Extra Selection
	public static native void MBClearExtraSelection();

	/// Apply Extra Selection to the current selection
	public static native void MBExtraSelection();

	/// Connect FROM Extra Selection
	public static native void MBConnectFromExtra();

	/// Connect TO Extra Selection
	public static native void MBConnectToExtra();

	/// Add an input neuron to the net
	public static native void MBAddInput(int posX, int posY, String name);

	/// Add an output neuron to the net
	public static native void MBAddOutput(int posX, int posY, String name);

	/// Add an unresolved hidden neuron to the net
	public static native void MBAddHidden(int posX, int posY, String name);

	/// Get the properties of the currently selected neuron
	public static native void MBGetSelectedNeuronProp(MBNeuronProp prop);

	/// Set the properties of the selected neurons
	public static native void MBSetSelectedNeuronProp(MBNeuronProp prop);

	/// Delete the selected objects
	public static native void MBDeleteSelectedObjects();
	
	/// Prepare for a new net
	public static native void MBClearNet();

	/// Set the name of the selected neurons  
	public static native void MBSetSelectedNeuronName(String name);

	/// Move all selected neurons
	public static native void MBMoveSelectedNeurons(int dX, int dY);
	  
	/// Select all links from Extra Selection to Selection
	public static native void MBSelectFromExtra();
	
	/// Select all links from Selection to Extra Selection
	public static native void MBSelectToExtra();
	
	
	/// Get the properties of the selected link
	public static native void MBGetSelectedLinkProp(MBLinkProp prop);
	
	/// Set the properties of the selected links
	public static native void MBSetSelectedLinkProp(MBLinkProp prop);
	
	/// Get the position of the selected neuron
	public static native void MBGetSelectedNeuronPos(int[] x, int[] y);
	
	/// Get the nearest grid point to a given point    
	public static native void MBGetGridPoint(int x, int y, int[] gridX, int[] gridY);
	
	/// Get the adjusted grid width    
	public static native int MBGetGridWidth();
	
	/// Get a random value between 0 an 1    
	public static native double MBRandom();
}
