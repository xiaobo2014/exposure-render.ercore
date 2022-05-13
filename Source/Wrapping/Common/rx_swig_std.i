
//
// Includes for STD-library support
//
%include <std_vector.i>
%include <std_string.i>
%include <std_array.i>
%include <std_map.i>


//%include <std_array.i>

#if SWIGPYTHON || SWIGRUBY
%include <std_complex.i>
#endif
// Use C99 int support
%include <stdint.i>

%include <std_array.i>
%include <std_vector.i>
%include <std_string.i>
using std::array;
using std::vector;
using std::string;

// Template definition for the most common vector types

namespace std {
  %template(VectorBool) vector<bool>;
  %template(VectorUInt8) vector<uint8_t>;
  %template(VectorInt8) vector<int8_t>;
  %template(VectorUInt16) vector<uint16_t>;
  %template(VectorInt16) vector<int16_t>;
  %template(VectorUInt32) vector<uint32_t>;
  %template(VectorInt32) vector<int32_t>;
  %template(VectorUInt64) vector<uint64_t>;
  %template(VectorInt64) vector<int64_t>;
  %template(VectorFloat) vector<float>;
  %template(VectorDouble) vector<double>;
  %template(VectorUIntList) vector< vector<unsigned int> >;
  %template(VectorString) vector< std::string >;

  %template(MapDoubleDouble) map<double, double>;
  %template(MapStringDouble) map<std::string, double>;
  %template(MapStringString) map<std::string, std::string>;

  %template(PairDoubleDouble) pair<double, double>;
  %template(PairStringDouble) pair<std::string, double>;

  %template(VectorMapStringDouble) vector< map< std::string, double> >;
  %template(VectorPairStringDouble) vector< pair< std::string, double> >;

  %template(VectorVectorDouble) vector<vector<double>>;

  //array
  %template(ArrayDouble) array<double, 3>;
  %template(VectorPoint) vector<array<double, 3>>;
}
