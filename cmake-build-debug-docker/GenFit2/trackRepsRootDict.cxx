// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME trackRepsRootDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "/tmp/source/GenFit2/trackReps/include/RKTrackRep.h"
#include "/tmp/source/GenFit2/trackReps/include/MplTrackRep.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace genfit {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *genfit_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("genfit", 0 /*version*/, "AbsFinitePlane.h", 35,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &genfit_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *genfit_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_genfitcLcLRKTrackRep(void *p = nullptr);
   static void *newArray_genfitcLcLRKTrackRep(Long_t size, void *p);
   static void delete_genfitcLcLRKTrackRep(void *p);
   static void deleteArray_genfitcLcLRKTrackRep(void *p);
   static void destruct_genfitcLcLRKTrackRep(void *p);
   static void streamer_genfitcLcLRKTrackRep(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::RKTrackRep*)
   {
      ::genfit::RKTrackRep *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::RKTrackRep >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::RKTrackRep", ::genfit::RKTrackRep::Class_Version(), "RKTrackRep.h", 72,
                  typeid(::genfit::RKTrackRep), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::RKTrackRep::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::RKTrackRep) );
      instance.SetNew(&new_genfitcLcLRKTrackRep);
      instance.SetNewArray(&newArray_genfitcLcLRKTrackRep);
      instance.SetDelete(&delete_genfitcLcLRKTrackRep);
      instance.SetDeleteArray(&deleteArray_genfitcLcLRKTrackRep);
      instance.SetDestructor(&destruct_genfitcLcLRKTrackRep);
      instance.SetStreamerFunc(&streamer_genfitcLcLRKTrackRep);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::RKTrackRep*)
   {
      return GenerateInitInstanceLocal((::genfit::RKTrackRep*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::RKTrackRep*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLMplTrackRep(void *p = nullptr);
   static void *newArray_genfitcLcLMplTrackRep(Long_t size, void *p);
   static void delete_genfitcLcLMplTrackRep(void *p);
   static void deleteArray_genfitcLcLMplTrackRep(void *p);
   static void destruct_genfitcLcLMplTrackRep(void *p);
   static void streamer_genfitcLcLMplTrackRep(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::MplTrackRep*)
   {
      ::genfit::MplTrackRep *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::MplTrackRep >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::MplTrackRep", ::genfit::MplTrackRep::Class_Version(), "MplTrackRep.h", 33,
                  typeid(::genfit::MplTrackRep), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::MplTrackRep::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::MplTrackRep) );
      instance.SetNew(&new_genfitcLcLMplTrackRep);
      instance.SetNewArray(&newArray_genfitcLcLMplTrackRep);
      instance.SetDelete(&delete_genfitcLcLMplTrackRep);
      instance.SetDeleteArray(&deleteArray_genfitcLcLMplTrackRep);
      instance.SetDestructor(&destruct_genfitcLcLMplTrackRep);
      instance.SetStreamerFunc(&streamer_genfitcLcLMplTrackRep);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::MplTrackRep*)
   {
      return GenerateInitInstanceLocal((::genfit::MplTrackRep*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::MplTrackRep*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr RKTrackRep::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *RKTrackRep::Class_Name()
{
   return "genfit::RKTrackRep";
}

//______________________________________________________________________________
const char *RKTrackRep::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::RKTrackRep*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int RKTrackRep::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::RKTrackRep*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RKTrackRep::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::RKTrackRep*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RKTrackRep::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::RKTrackRep*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr MplTrackRep::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MplTrackRep::Class_Name()
{
   return "genfit::MplTrackRep";
}

//______________________________________________________________________________
const char *MplTrackRep::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::MplTrackRep*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MplTrackRep::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::MplTrackRep*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MplTrackRep::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::MplTrackRep*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MplTrackRep::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::MplTrackRep*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLRKTrackRep(void *p) {
      return  p ? new(p) ::genfit::RKTrackRep : new ::genfit::RKTrackRep;
   }
   static void *newArray_genfitcLcLRKTrackRep(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::RKTrackRep[nElements] : new ::genfit::RKTrackRep[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLRKTrackRep(void *p) {
      delete ((::genfit::RKTrackRep*)p);
   }
   static void deleteArray_genfitcLcLRKTrackRep(void *p) {
      delete [] ((::genfit::RKTrackRep*)p);
   }
   static void destruct_genfitcLcLRKTrackRep(void *p) {
      typedef ::genfit::RKTrackRep current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLRKTrackRep(TBuffer &buf, void *obj) {
      ((::genfit::RKTrackRep*)obj)->::genfit::RKTrackRep::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::RKTrackRep

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLMplTrackRep(void *p) {
      return  p ? new(p) ::genfit::MplTrackRep : new ::genfit::MplTrackRep;
   }
   static void *newArray_genfitcLcLMplTrackRep(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::MplTrackRep[nElements] : new ::genfit::MplTrackRep[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLMplTrackRep(void *p) {
      delete ((::genfit::MplTrackRep*)p);
   }
   static void deleteArray_genfitcLcLMplTrackRep(void *p) {
      delete [] ((::genfit::MplTrackRep*)p);
   }
   static void destruct_genfitcLcLMplTrackRep(void *p) {
      typedef ::genfit::MplTrackRep current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLMplTrackRep(TBuffer &buf, void *obj) {
      ((::genfit::MplTrackRep*)obj)->::genfit::MplTrackRep::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::MplTrackRep

namespace {
  void TriggerDictionaryInitialization_libtrackRepsRootDict_Impl() {
    static const char* headers[] = {
"/tmp/source/GenFit2/trackReps/include/RKTrackRep.h",
"/tmp/source/GenFit2/trackReps/include/MplTrackRep.h",
nullptr
    };
    static const char* includePaths[] = {
"/tmp/source/Utility/Object/include",
"/tmp/source/GenFit2/./trackReps/include",
"/tmp/source/GenFit2/./measurements/include",
"/tmp/source/GenFit2/./fitters/include",
"/tmp/source/GenFit2/./fields/include",
"/tmp/source/GenFit2/./core/include",
"/usr/include/eigen3",
"/opt/root/include",
"/opt/geant4/include/Geant4",
"/usr/include",
"/tmp/source/.",
"/tmp/source/GenFit2/core/include",
"/tmp/source/GenFit2/fields/include",
"/tmp/source/GenFit2/fitters/include",
"/tmp/source/GenFit2/measurements/include",
"/tmp/source/GenFit2/trackReps/include",
"/tmp/source/Utility/Object/include/Object",
"/tmp/source/Utility/UTIL/include",
"/tmp/source/Algorithms/CutFlowAnalysis/include",
"/tmp/source/Algorithms/Digitizer/include",
"/tmp/source/Algorithms/ExampleProcessor/include",
"/tmp/source/Algorithms/MCTruthAnalysis/include",
"/tmp/source/Algorithms/RecECAL/include",
"/tmp/source/Algorithms/Tracking/include",
"/tmp/source/Algorithms/Tracking/include/Algo",
"/tmp/source/GenFit2",
"/opt/root/include/",
"/tmp/source/cmake-build-debug-docker/GenFit2/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libtrackRepsRootDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/trackReps/include/RKTrackRep.h")))  RKTrackRep;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/trackReps/include/MplTrackRep.h")))  MplTrackRep;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libtrackRepsRootDict dictionary payload"

#ifndef G4UI_USE_TCSH
  #define G4UI_USE_TCSH 1
#endif
#ifndef G4INTY_USE_QT
  #define G4INTY_USE_QT 1
#endif
#ifndef G4UI_USE_QT
  #define G4UI_USE_QT 1
#endif
#ifndef G4VIS_USE_OPENGLQT
  #define G4VIS_USE_OPENGLQT 1
#endif
#ifndef G4INTY_USE_XT
  #define G4INTY_USE_XT 1
#endif
#ifndef G4UI_USE_XM
  #define G4UI_USE_XM 1
#endif
#ifndef G4VIS_USE_OPENGLXM
  #define G4VIS_USE_OPENGLXM 1
#endif
#ifndef G4VIS_USE_OPENGLX
  #define G4VIS_USE_OPENGLX 1
#endif
#ifndef G4VIS_USE_OPENGL
  #define G4VIS_USE_OPENGL 1
#endif
#ifndef DEBUG
  #define DEBUG 1
#endif
#ifndef MEMCK
  #define MEMCK 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/tmp/source/GenFit2/trackReps/include/RKTrackRep.h"
#include "/tmp/source/GenFit2/trackReps/include/MplTrackRep.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"genfit::MplTrackRep", payloadCode, "@",
"genfit::RKTrackRep", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libtrackRepsRootDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libtrackRepsRootDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libtrackRepsRootDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libtrackRepsRootDict() {
  TriggerDictionaryInitialization_libtrackRepsRootDict_Impl();
}
