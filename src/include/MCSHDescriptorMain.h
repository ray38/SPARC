void taskPartition(const int length, const double *rCutoffList, const int *lList, const int *groupList, const int numParallelComm, int *taskAssignmentList);

void MCSHDescriptorMainFixed(const double *rho, const int imageDimX, const int imageDimY, const int imageDimZ, const double hx, const double hy, const double hz, 
						double *U, const int accuracy, const int commIndex, const int numParallelComm, const MPI_Comm communicator);

void MCSHDescriptorMain(const double *rho, const int imageDimX, const int imageDimY, const int imageDimZ, const double hx, const double hy, const double hz, 
						double *U, const int accuracy, const int maxOrder, const double rMaxCutoff, const double rStepsize, 
						const int commIndex, const int numParallelComm, const MPI_Comm communicator);

void calcAndSaveCoords(const double *rho, const int imageDimX, const int imageDimY, const int imageDimZ, const double hx, const double hy, const double hz, double *U);