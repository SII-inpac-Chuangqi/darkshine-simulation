// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__Display
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
#include "/tmp/source/DP_display/include/DEventDisplay.h"
#include "/tmp/source/DP_display/include/DEventReader_dis.h"
#include "/tmp/source/DP_display/include/DSMagneticField.h"
#include "/tmp/source/DP_display/include/CaloHitsDisplay.h"
#include "/tmp/source/DP_display/include/DisData.h"
#include "/tmp/source/DP_ana/include/Event/EventReader.h"
#include "/tmp/source/DP_ana/include/Event/EventStoreAndWriter.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *DisData_Dictionary();
   static void DisData_TClassManip(TClass*);
   static void delete_DisData(void *p);
   static void deleteArray_DisData(void *p);
   static void destruct_DisData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DisData*)
   {
      ::DisData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::DisData));
      static ::ROOT::TGenericClassInfo 
         instance("DisData", "DisData.h", 11,
                  typeid(::DisData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &DisData_Dictionary, isa_proxy, 4,
                  sizeof(::DisData) );
      instance.SetDelete(&delete_DisData);
      instance.SetDeleteArray(&deleteArray_DisData);
      instance.SetDestructor(&destruct_DisData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DisData*)
   {
      return GenerateInitInstanceLocal((::DisData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DisData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *DisData_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::DisData*)nullptr)->GetClass();
      DisData_TClassManip(theClass);
   return theClass;
   }

   static void DisData_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *DSMagneticField_Dictionary();
   static void DSMagneticField_TClassManip(TClass*);
   static void *new_DSMagneticField(void *p = nullptr);
   static void *newArray_DSMagneticField(Long_t size, void *p);
   static void delete_DSMagneticField(void *p);
   static void deleteArray_DSMagneticField(void *p);
   static void destruct_DSMagneticField(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DSMagneticField*)
   {
      ::DSMagneticField *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::DSMagneticField));
      static ::ROOT::TGenericClassInfo 
         instance("DSMagneticField", "DSMagneticField.h", 14,
                  typeid(::DSMagneticField), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &DSMagneticField_Dictionary, isa_proxy, 4,
                  sizeof(::DSMagneticField) );
      instance.SetNew(&new_DSMagneticField);
      instance.SetNewArray(&newArray_DSMagneticField);
      instance.SetDelete(&delete_DSMagneticField);
      instance.SetDeleteArray(&deleteArray_DSMagneticField);
      instance.SetDestructor(&destruct_DSMagneticField);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DSMagneticField*)
   {
      return GenerateInitInstanceLocal((::DSMagneticField*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DSMagneticField*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *DSMagneticField_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::DSMagneticField*)nullptr)->GetClass();
      DSMagneticField_TClassManip(theClass);
   return theClass;
   }

   static void DSMagneticField_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CaloHitsDisplay(void *p = nullptr);
   static void *newArray_CaloHitsDisplay(Long_t size, void *p);
   static void delete_CaloHitsDisplay(void *p);
   static void deleteArray_CaloHitsDisplay(void *p);
   static void destruct_CaloHitsDisplay(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CaloHitsDisplay*)
   {
      ::CaloHitsDisplay *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CaloHitsDisplay >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CaloHitsDisplay", ::CaloHitsDisplay::Class_Version(), "CaloHitsDisplay.h", 50,
                  typeid(::CaloHitsDisplay), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CaloHitsDisplay::Dictionary, isa_proxy, 4,
                  sizeof(::CaloHitsDisplay) );
      instance.SetNew(&new_CaloHitsDisplay);
      instance.SetNewArray(&newArray_CaloHitsDisplay);
      instance.SetDelete(&delete_CaloHitsDisplay);
      instance.SetDeleteArray(&deleteArray_CaloHitsDisplay);
      instance.SetDestructor(&destruct_CaloHitsDisplay);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CaloHitsDisplay*)
   {
      return GenerateInitInstanceLocal((::CaloHitsDisplay*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CaloHitsDisplay*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DEventDisplay(void *p = nullptr);
   static void *newArray_DEventDisplay(Long_t size, void *p);
   static void delete_DEventDisplay(void *p);
   static void deleteArray_DEventDisplay(void *p);
   static void destruct_DEventDisplay(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DEventDisplay*)
   {
      ::DEventDisplay *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DEventDisplay >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DEventDisplay", ::DEventDisplay::Class_Version(), "DEventDisplay.h", 57,
                  typeid(::DEventDisplay), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DEventDisplay::Dictionary, isa_proxy, 4,
                  sizeof(::DEventDisplay) );
      instance.SetNew(&new_DEventDisplay);
      instance.SetNewArray(&newArray_DEventDisplay);
      instance.SetDelete(&delete_DEventDisplay);
      instance.SetDeleteArray(&deleteArray_DEventDisplay);
      instance.SetDestructor(&destruct_DEventDisplay);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DEventDisplay*)
   {
      return GenerateInitInstanceLocal((::DEventDisplay*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DEventDisplay*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CaloHitsDisplay::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CaloHitsDisplay::Class_Name()
{
   return "CaloHitsDisplay";
}

//______________________________________________________________________________
const char *CaloHitsDisplay::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CaloHitsDisplay*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CaloHitsDisplay::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CaloHitsDisplay*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CaloHitsDisplay::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CaloHitsDisplay*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CaloHitsDisplay::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CaloHitsDisplay*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DEventDisplay::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DEventDisplay::Class_Name()
{
   return "DEventDisplay";
}

//______________________________________________________________________________
const char *DEventDisplay::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DEventDisplay*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DEventDisplay::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DEventDisplay*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DEventDisplay::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DEventDisplay*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DEventDisplay::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DEventDisplay*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_DisData(void *p) {
      delete ((::DisData*)p);
   }
   static void deleteArray_DisData(void *p) {
      delete [] ((::DisData*)p);
   }
   static void destruct_DisData(void *p) {
      typedef ::DisData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DisData

namespace ROOT {
   // Wrappers around operator new
   static void *new_DSMagneticField(void *p) {
      return  p ? new(p) ::DSMagneticField : new ::DSMagneticField;
   }
   static void *newArray_DSMagneticField(Long_t nElements, void *p) {
      return p ? new(p) ::DSMagneticField[nElements] : new ::DSMagneticField[nElements];
   }
   // Wrapper around operator delete
   static void delete_DSMagneticField(void *p) {
      delete ((::DSMagneticField*)p);
   }
   static void deleteArray_DSMagneticField(void *p) {
      delete [] ((::DSMagneticField*)p);
   }
   static void destruct_DSMagneticField(void *p) {
      typedef ::DSMagneticField current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DSMagneticField

//______________________________________________________________________________
void CaloHitsDisplay::Streamer(TBuffer &R__b)
{
   // Stream an object of class CaloHitsDisplay.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CaloHitsDisplay::Class(),this);
   } else {
      R__b.WriteClassBuffer(CaloHitsDisplay::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CaloHitsDisplay(void *p) {
      return  p ? new(p) ::CaloHitsDisplay : new ::CaloHitsDisplay;
   }
   static void *newArray_CaloHitsDisplay(Long_t nElements, void *p) {
      return p ? new(p) ::CaloHitsDisplay[nElements] : new ::CaloHitsDisplay[nElements];
   }
   // Wrapper around operator delete
   static void delete_CaloHitsDisplay(void *p) {
      delete ((::CaloHitsDisplay*)p);
   }
   static void deleteArray_CaloHitsDisplay(void *p) {
      delete [] ((::CaloHitsDisplay*)p);
   }
   static void destruct_CaloHitsDisplay(void *p) {
      typedef ::CaloHitsDisplay current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CaloHitsDisplay

//______________________________________________________________________________
void DEventDisplay::Streamer(TBuffer &R__b)
{
   // Stream an object of class DEventDisplay.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DEventDisplay::Class(),this);
   } else {
      R__b.WriteClassBuffer(DEventDisplay::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DEventDisplay(void *p) {
      return  p ? new(p) ::DEventDisplay : new ::DEventDisplay;
   }
   static void *newArray_DEventDisplay(Long_t nElements, void *p) {
      return p ? new(p) ::DEventDisplay[nElements] : new ::DEventDisplay[nElements];
   }
   // Wrapper around operator delete
   static void delete_DEventDisplay(void *p) {
      delete ((::DEventDisplay*)p);
   }
   static void deleteArray_DEventDisplay(void *p) {
      delete [] ((::DEventDisplay*)p);
   }
   static void destruct_DEventDisplay(void *p) {
      typedef ::DEventDisplay current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DEventDisplay

namespace {
  void TriggerDictionaryInitialization_libDisplay_Impl() {
    static const char* headers[] = {
"/tmp/source/DP_display/include/DEventDisplay.h",
"/tmp/source/DP_display/include/DEventReader_dis.h",
"/tmp/source/DP_display/include/DSMagneticField.h",
"/tmp/source/DP_display/include/CaloHitsDisplay.h",
"/tmp/source/DP_display/include/DisData.h",
"/tmp/source/DP_ana/include/Event/EventReader.h",
"/tmp/source/DP_ana/include/Event/EventStoreAndWriter.h",
nullptr
    };
    static const char* includePaths[] = {
"/opt/geant4/include/Geant4",
"/usr/include",
"/usr/include/eigen3",
"/opt/root/include",
"/tmp/source/.",
"/tmp/source/GenFit2/core/include",
"/tmp/source/GenFit2/fields/include",
"/tmp/source/GenFit2/fitters/include",
"/tmp/source/GenFit2/measurements/include",
"/tmp/source/GenFit2/trackReps/include",
"/tmp/source/Utility/Object/include",
"/tmp/source/Utility/Object/include/Object",
"/tmp/source/Utility/UTIL/include",
"/tmp/source/Algorithms/CutFlowAnalysis/include",
"/tmp/source/Algorithms/Digitizer/include",
"/tmp/source/Algorithms/ExampleProcessor/include",
"/tmp/source/Algorithms/MCTruthAnalysis/include",
"/tmp/source/Algorithms/RecECAL/include",
"/tmp/source/Algorithms/Tracking/include",
"/tmp/source/Algorithms/Tracking/include/Algo",
"/tmp/source/DP_display/include",
"/tmp/source/DP_ana/include",
"/tmp/source/DP_display",
"/opt/root/include/",
"/tmp/source/cmake-build-debug-docker/DP_display/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libDisplay dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$DisData.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/DP_display/include/DEventDisplay.h")))  DisData;
class __attribute__((annotate("$clingAutoload$DSMagneticField.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/DP_display/include/DEventDisplay.h")))  DSMagneticField;
class __attribute__((annotate("$clingAutoload$CaloHitsDisplay.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/DP_display/include/DEventDisplay.h")))  CaloHitsDisplay;
class __attribute__((annotate("$clingAutoload$/tmp/source/DP_display/include/DEventDisplay.h")))  DEventDisplay;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libDisplay dictionary payload"

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
#include "/tmp/source/DP_display/include/DEventDisplay.h"
#include "/tmp/source/DP_display/include/DEventReader_dis.h"
#include "/tmp/source/DP_display/include/DSMagneticField.h"
#include "/tmp/source/DP_display/include/CaloHitsDisplay.h"
#include "/tmp/source/DP_display/include/DisData.h"
#include "/tmp/source/DP_ana/include/Event/EventReader.h"
#include "/tmp/source/DP_ana/include/Event/EventStoreAndWriter.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CaloHitsDisplay", payloadCode, "@",
"DEventDisplay", payloadCode, "@",
"DSMagneticField", payloadCode, "@",
"DisData", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libDisplay",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libDisplay_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libDisplay_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libDisplay() {
  TriggerDictionaryInitialization_libDisplay_Impl();
}
