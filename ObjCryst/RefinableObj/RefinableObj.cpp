/*
* ObjCryst++ : a Crystallographic computing library in C++
*
*  (c) 2000 Vincent FAVRE-NICOLIN
*           Laboratoire de Cristallographie
*           24, quai Ernest-Ansermet, CH-1211 Geneva 4, Switzerland
*  Contact: Vincent.Favre-Nicolin@cryst.unige.ch
*           Radovan.Cerny@cryst.unige.ch
*
*/
/*
*  source file for the RefinablePar and RefinableObj classes
*
*/
#include "RefinableObj/RefinableObj.h"
#include "Quirks/VFNStreamFormat.h"
#include "Quirks/VFNDebug.h"
#ifdef __WX__CRYST__
   #include "wxCryst/wxRefinableObj.h"
   #undef GetClassName // Conflict from wxMSW headers ? (cygwin)
#endif
namespace ObjCryst
{
//######################################################################
//
//      RefParType
//
//######################################################################
RefParType::RefParType(const string &name):
mpParent(0),mName(name)
{
}

RefParType::RefParType(const RefParType *parent,const string &name):
mpParent(parent),mName(name)
{}

RefParType::~RefParType(){}; 

bool RefParType::IsDescendantFromOrSameAs(const RefParType *type) const
{
   VFN_DEBUG_MESSAGE("RefParType::IsDescendantFromOrSameAs(RefParType*): "<<this<<" : "<<mpParent,1)
   if(this==type) return true;
   if(0==mpParent) return false;
   return mpParent->IsDescendantFromOrSameAs(type);
}

bool RefParType::operator==(const RefParType *type) const
{
   if(this==type) return true;
   return false;
}
const string& RefParType::GetName() const{ return mName;}

const RefParType *gpRefParTypeObjCryst=new RefParType("ObjCryst++");

//######################################################################
//
//      
//
//######################################################################

unsigned long RefinableObjClock::msTick0=0;
unsigned long RefinableObjClock::msTick1=0;
RefinableObjClock::RefinableObjClock()
{
   //this->Click();
   mTick0=0;
   mTick1=0;
}
RefinableObjClock::~RefinableObjClock(){}

bool RefinableObjClock::operator< (const RefinableObjClock &rhs)const
{
   if(mTick1<rhs.mTick1) return true;
   if(mTick1==rhs.mTick1) {if(mTick0<rhs.mTick0) return true;}
   return false;
}
bool RefinableObjClock::operator<=(const RefinableObjClock &rhs)const
{
   if(mTick1<rhs.mTick1) return true;
   if(mTick1==rhs.mTick1) if(mTick0<=rhs.mTick0) return true;
   return false;
}
bool RefinableObjClock::operator> (const RefinableObjClock &rhs)const
{
   if(mTick1>rhs.mTick1) return true;
   if(mTick1==rhs.mTick1) if(mTick0>rhs.mTick0) return true;
   return false;
}
bool RefinableObjClock::operator>=(const RefinableObjClock &rhs)const
{
   if(mTick1>rhs.mTick1) return true;
   if(mTick1==rhs.mTick1) if(mTick0>=rhs.mTick0) return true;
   return false;
}
void RefinableObjClock::Click()
{
   //return;
   if(++msTick0==0) ++msTick1;//Update ObjCryst++ static event counter
   mTick0=msTick0;
   mTick1=msTick1;
   VFN_DEBUG_MESSAGE("RefinableObjClock::Click():"<<mTick1<<":"<<mTick0<<"(at "<<this<<")",0)
   //this->Print();
}
void RefinableObjClock::Reset()
{
   mTick0=0;
   mTick1=0;
}
void RefinableObjClock::Print()const
{
   cout <<"Clock():"<<mTick1<<":"<<mTick0;
   VFN_DEBUG_MESSAGE_SHORT(" (at "<<this<<")",4)
   cout <<endl;
}
void RefinableObjClock::PrintStatic()const
{
   cout <<"RefinableObj class Clock():"<<msTick1<<":"<<msTick0<<endl;
}

//######################################################################
//    RefinablePar
//######################################################################

RefinablePar::RefinablePar():mName(""),mpRefParType(0),mValue(0),mMin(0),mMax(0),
                     mHasLimits(false),mIsFixed(true),mIsUsed(true),mIsPeriodic(false),
                     mPeriod(0.),mHumanScale(1.),mHasAssignedClock(false),mpClock(0)
{}
                     
RefinablePar::RefinablePar(  const string &name,
                     double *refPar,
                     const double min,
                     const double max,
                     const RefParType *type,
                     RefParDerivStepModel derivMode,
                     const bool hasLimits,
                     const bool isFixed,
                     const bool isUsed,
                     const bool isPeriodic,
                     const double humanScale,
                     double period):
mName(name),mpRefParType(type),mValue(refPar),mMin(min),mMax(max),
mHasLimits(hasLimits),mIsFixed(isFixed),mIsUsed(isUsed),mIsPeriodic(isPeriodic),mPeriod(period),
mGlobalOptimStep((max-min)/100.),mDerivStep(1e-5),mRefParDerivStepModel(derivMode),
mSigma(0.),mHumanScale(humanScale),
mUseEquation(false),mEquationNbRefPar(0),mEquationCoeff(0),
mHasAssignedClock(false),mpClock(0)
{}

RefinablePar::~RefinablePar()
{}

void RefinablePar::Init(const string &name,
                        double *refPar,
                        const double min,
                        const double max,
                        const RefParType *type,
                        RefParDerivStepModel derivMode,
                        const bool hasLimits,
                        const bool isFixed,
                        const bool isUsed,
                        const bool isPeriodic,
                        const double humanScale,
                        double period)
{
   mName=name;
   mpRefParType=type;
   mValue=refPar;
   mMin=min;
   mMax=max;
   mHasLimits=hasLimits;
   mIsFixed=isFixed;
   mIsUsed=isUsed;
   mIsPeriodic=isPeriodic;
   mPeriod=period;
   mGlobalOptimStep=(max-min)/100.;
   mDerivStep=1e-5;
   mRefParDerivStepModel=derivMode;
   mSigma=0.;
   mHumanScale=humanScale;
   mUseEquation=false;
   mEquationNbRefPar=0;
   mEquationCoeff=0;
   mHasAssignedClock=false;
   mpClock=0;
}

      

double RefinablePar::GetValue()const
{
   if(false==mUseEquation) return *mValue;
   else
   {
      VFN_DEBUG_MESSAGE("RefinablePar::Value():Evaluating Equation",0)
      double tmp=mEquationCoeff(0);
      for(int i=0;i<mEquationNbRefPar;i++) 
         tmp += mEquationCoeff(i+1) * mEquationRefPar[i]->GetValue();
      *mValue = tmp;
   }
   return *mValue;
}

void RefinablePar::SetValue(const double value)
{
   this->Click();
   VFN_DEBUG_MESSAGE("RefinablePar::MutateTo()",2)
   if(true==mUseEquation)
   {
      cout << "RefinablePar::SetValue(): this parameter is defined by an equation !!" <<endl;
      throw 0;
   }
   *mValue = value;
   /*
   if(this->IsLimited() ==true)
   {
      if(true==this->IsPeriodic())
      {
         if(*mValue > this->GetMax()) *mValue -= this->GetMax()-this->GetMin();
         if(*mValue < this->GetMin()) *mValue += this->GetMax()-this->GetMin();
      }
      else
      {
         if(*mValue > this->GetMax()) *mValue=this->GetMax();
         if(*mValue < this->GetMin()) *mValue=this->GetMin();
      }
   }
   */
   if(this->IsLimited() ==true)
   {
      if(*mValue > this->GetMax()) *mValue=this->GetMax();
      if(*mValue < this->GetMin()) *mValue=this->GetMin();
   }
   else if(true==this->IsPeriodic())
   {
      if(*mValue > mPeriod) *mValue -= mPeriod;
      if(*mValue < 0) *mValue += mPeriod;
   }
}

const double& RefinablePar::GetHumanValue() const
{
   static double val;
   val = *mValue * mHumanScale;
   return val;
}

void RefinablePar::SetHumanValue(const double &value)
{
   this->Click();
   VFN_DEBUG_MESSAGE("RefinablePar::MutateTo()",2)
   if(true==mUseEquation)
   {
      cout << "RefinablePar::SetValue(): this parameter is defined by an equation !!" <<endl;
      throw 0;
   }
   *mValue = value/mHumanScale;
   /*
   if(this->IsLimited() ==true)
   {
      if(true==this->IsPeriodic())
      {
         if(*mValue > this->GetMax()) *mValue -= this->GetMax()-this->GetMin();
         if(*mValue < this->GetMin()) *mValue += this->GetMax()-this->GetMin();
      }
      else
      {
         if(*mValue > this->GetMax()) *mValue=this->GetMax();
         if(*mValue < this->GetMin()) *mValue=this->GetMin();
      }
   }
   */
   if(this->IsLimited() ==true)
   {
      if(*mValue > this->GetMax()) *mValue=this->GetMax();
      if(*mValue < this->GetMin()) *mValue=this->GetMin();
   }
   else if(true==this->IsPeriodic())
   {
      if(*mValue > mPeriod) *mValue -= mPeriod;
      if(*mValue < 0) *mValue += mPeriod;
   }
}

void RefinablePar::Mutate(const double mutateValue)
{
   VFN_DEBUG_MESSAGE("RefinablePar::Mutate():"<<this->GetName(),1)
   this->Click();
   if(true==mUseEquation)
   {
      cout << "RefinablePar::Mutate(): this parameter is defined by an equation !!" <<endl;
      throw 0;
   }
   *mValue += mutateValue;
   /*
   if(this->IsLimited() ==true)
   {
      if(true==this->IsPeriodic())
      {
         if(*mValue > this->GetMax()) *mValue -= this->GetMax()-this->GetMin();
         if(*mValue < this->GetMin()) *mValue += this->GetMax()-this->GetMin();
      }
      else
      {
         if(*mValue > this->GetMax()) *mValue=this->GetMax();
         if(*mValue < this->GetMin()) *mValue=this->GetMin();
      }
   }
   */
   if(this->IsLimited() ==true)
   {
      if(*mValue > this->GetMax()) *mValue=this->GetMax();
      if(*mValue < this->GetMin()) *mValue=this->GetMin();
   }
   else if(true==this->IsPeriodic())
   {
      if(*mValue > mPeriod) *mValue -= mPeriod;
      if(*mValue < 0) *mValue += mPeriod;
   }
   VFN_DEBUG_MESSAGE("RefinablePar::Mutate():End",0)
}

void RefinablePar::MutateTo(const double mutateValue)
{
   VFN_DEBUG_MESSAGE("RefinablePar::MutateTo()",2)
   this->Click();
   if(true==mUseEquation)
   {
      cout << "RefinablePar::Mutate(): this parameter is defined by an equation !!" <<endl;
      throw 0;
   }
   *mValue = mutateValue;
   /*
   if(this->IsLimited() ==true)
   {
      if(true==this->IsPeriodic())
      {
         if(*mValue > this->GetMax()) *mValue -= this->GetMax()-this->GetMin();
         if(*mValue < this->GetMin()) *mValue += this->GetMax()-this->GetMin();
      }
      else
      {
         if(*mValue > this->GetMax()) *mValue=this->GetMax();
         if(*mValue < this->GetMin()) *mValue=this->GetMin();
      }
   }
   */
   if(this->IsLimited() ==true)
   {
      if(*mValue > this->GetMax()) *mValue=this->GetMax();
      if(*mValue < this->GetMin()) *mValue=this->GetMin();
   }
   else if(true==this->IsPeriodic())
   {
      if(*mValue > mPeriod) *mValue -= mPeriod;
      if(*mValue < 0) *mValue += mPeriod;
   }
}

double  RefinablePar::GetSigma()const {return mSigma;}
double  RefinablePar::GetHumanSigma()const {return mSigma*mHumanScale;}
void   RefinablePar::SetSigma(const double sigma) {mSigma=sigma; this->Click();}

const RefParType*  RefinablePar::GetType()const {return mpRefParType;}
void RefinablePar::SetType(const RefParType *type) { mpRefParType=type;}

void RefinablePar::Print() const
{
      cout << this->GetName() << " : " << this->GetHumanValue()
			  << " Fixed:"<< mIsFixed <<" Periodic:"<<mIsPeriodic<<" Limited:"<<mHasLimits
           << " Min:" << this->GetHumanMin() << " Max:" << this->GetHumanMax()
      #ifdef __DEBUG__
           << ",HasClock=" << mHasAssignedClock << " at " << mpClock
      #endif
           <<endl;
}

string RefinablePar::GetName()const {return mName;}

bool RefinablePar::IsFixed()const {return mIsFixed;}
void RefinablePar::SetIsFixed(const bool b)
{
   VFN_DEBUG_MESSAGE("RefinablePar::SetIsFixed():"<<this->GetName(),1)
   mIsFixed=b;
}

bool RefinablePar::IsLimited()const {return mHasLimits;}
void RefinablePar::SetIsLimited(const bool b) {mHasLimits=b;this->Click();}

bool RefinablePar::IsUsed()const {return mIsUsed;}
void RefinablePar::SetIsUsed(const bool b) {mIsUsed=b;this->Click();}

bool RefinablePar::IsPeriodic()const {return mIsPeriodic;}
void RefinablePar::SetIsPeriodic(const bool b,double period)
{mIsPeriodic=b;mPeriod=period;this->Click();}


double RefinablePar::GetMin()const   {return mMin;}
void  RefinablePar::SetMin(const double min) { mMin=min;this->Click();}
double RefinablePar::GetHumanMin()const   {return mMin * mHumanScale;}
void  RefinablePar::SetHumanMin(const double min) { mMin=min/mHumanScale;this->Click();}

double RefinablePar::GetMax()const   {return mMax;}
void  RefinablePar::SetMax(const double max) { mMax=max;this->Click();}
double RefinablePar::GetHumanMax()const   {return mMax * mHumanScale;}
void  RefinablePar::SetHumanMax(const double max) { mMax=max/mHumanScale;this->Click();}

double RefinablePar::GetPeriod()const   {return mPeriod;}
void  RefinablePar::SetPeriod(const double period)
{ mPeriod=period;this->Click();}

double  RefinablePar::GetDerivStep()const
{
   if(REFPAR_DERIV_STEP_ABSOLUTE==mRefParDerivStepModel) return mDerivStep;
   double d=mDerivStep* (*mValue);
   
   //:KLUDGE: Parameter will probably has a singular value, so it should not matter..
   if(d == 0.) return 1e-8;
   return d;
}

void RefinablePar::SetDerivStep(const double step)
{
   this->Click();
   mDerivStep = step;
}

double RefinablePar::GetGlobalOptimStep()const {return mGlobalOptimStep;}
void  RefinablePar::SetGlobalOptimStep(const double step) {mGlobalOptimStep=step;}

double RefinablePar::GetHumanScale()const {return mHumanScale;}
void  RefinablePar::SetHumanScale(const double scale) {mHumanScale=scale;}

void RefinablePar::SetUseEquation(const bool useItOrNot,const double c0)
{
   this->Click();
   mUseEquation=useItOrNot;
   if(true==mUseEquation)
   {
      mEquationCoeff.resize(mEquationMaxRefPar);
      mEquationCoeff(0)=c0;
   }
}

void RefinablePar::SetUseEquation(const bool useItOrNot,const double c0,
                                  const double c1, const RefinablePar &refpar1)
{
   this->Click();
   mUseEquation=useItOrNot;
   if(true==mUseEquation)
   {
      mEquationCoeff.resize(mEquationMaxRefPar);
      mEquationCoeff(0)=c0;
      mEquationCoeff(1)=c1;
      mEquationRefPar[0]=&refpar1;
   }
}

void RefinablePar::SetUseEquation(const bool useItOrNot,const double c0,
                                  const double c1, const RefinablePar &refpar1,
                                  const double c2, const RefinablePar &refpar2)
{
   this->Click();
   mUseEquation=useItOrNot;
   if(true==mUseEquation)
   {
      mEquationCoeff.resize(mEquationMaxRefPar);
      mEquationCoeff(0)=c0;
      mEquationCoeff(1)=c1;
      mEquationCoeff(2)=c2;
      mEquationRefPar[0]=&refpar1;
      mEquationRefPar[1]=&refpar2;
   }
}

void RefinablePar::SetUseEquation(const bool useItOrNot,const double c0,
                                  const double c1, const RefinablePar &refpar1,
                                  const double c2, const RefinablePar &refpar2,
                                  const double c3, const RefinablePar &refpar3)
{
   this->Click();
   mUseEquation=useItOrNot;
   if(true==mUseEquation)
   {
      mEquationCoeff.resize(mEquationMaxRefPar);
      mEquationCoeff(0)=c0;
      mEquationCoeff(1)=c1;
      mEquationCoeff(2)=c2;
      mEquationCoeff(3)=c2;
      mEquationRefPar[0]=&refpar1;
      mEquationRefPar[1]=&refpar2;
      mEquationRefPar[2]=&refpar2;
   }
}
void RefinablePar::AssignClock(RefinableObjClock &clock)
{
   VFN_DEBUG_MESSAGE("RefinablePar::AssignClock() for "<<this->GetName()<< "at "<<&clock,4)
   mpClock=&clock;
   mHasAssignedClock=true;
}
void RefinablePar::Click()
{
   if(false==mHasAssignedClock) return;
   VFN_DEBUG_MESSAGE("RefinablePar::Click():"<<this->GetName(),1)
   mpClock->Click();
   //mpClock->Print();
   //VFN_DEBUG_MESSAGE("RefinablePar::Click():End",2)
}

void RefinablePar::SetLimitsAbsolute(const double min, const double max)
{
   //:TODO: check limits
   mMin=min;
   mMax=max;
   mHasLimits=true;
}
void RefinablePar::SetLimitsRelative(const double min, const double max)
{
   VFN_DEBUG_MESSAGE("RefinablePar::SetLimitsRelative():"<<this->GetName(),1)
   //:TODO: check limits
   mMin=this->GetValue()+min;
   mMax=this->GetValue()+max;
   mHasLimits=true;
}
void RefinablePar::SetLimitsProportional(const double min, const double max)
{
   //:TODO: check limits
   mMin=this->GetValue()*min;
   mMax=this->GetValue()*max;
   mHasLimits=true;
}
//######################################################################
//    RefObjOpt
//######################################################################
RefObjOpt::RefObjOpt()
{
   #ifdef __WX__CRYST__
   mpWXFieldOption=0;
   #endif
}

RefObjOpt::~RefObjOpt()
{}

void RefObjOpt::Init(const int nbChoice,
                     const string *name,
                     const string *choiceNames)
{
   VFN_DEBUG_MESSAGE("RefObjOpt::Init()"<<*name,5)
   mNbChoice=nbChoice;
   mChoice=0;
   mpName=name;
   mpChoiceName=choiceNames;
}

int RefObjOpt::GetNbChoice()const
{ return mNbChoice;}

int RefObjOpt::GetChoice()const
{ return mChoice;}

void RefObjOpt::SetChoice(const int choice)
{
   if(mChoice==choice)return;
   VFN_DEBUG_MESSAGE("RefObjOpt::SetChoice()"<<this->GetName()<< \
                     " to "<<this->GetChoiceName(choice),5)
   mChoice=choice;
   mClock.Click();
}
void RefObjOpt::SetChoice(const string &choiceName)
{
   int choice;
   for(choice=0;choice<mNbChoice;choice++) if(choiceName==*(mpChoiceName+choice)) break;
   if(choice==mNbChoice) choice=0;
   this->SetChoice(choice);
}

const string& RefObjOpt::GetName()const
{
   return *mpName;
}

const string& RefObjOpt::GetClassName()const
{
   static string className="Option";
   return className;
}

const string& RefObjOpt::GetChoiceName(const int i)const
{
   return *(mpChoiceName+i);
}

const RefinableObjClock& RefObjOpt::GetClock()const{return mClock;}

#ifdef __WX__CRYST__
WXCrystObjBasic* RefObjOpt::WXCreate(wxWindow *parent)
{
   VFN_DEBUG_MESSAGE("RefObjOpt::WXCreate()",10)
   mpWXFieldOption=new WXFieldOption (parent,-1,this);
   return mpWXFieldOption;
}
WXCrystObjBasic* RefObjOpt::WXGet()
{
   return mpWXFieldOption;
}
void RefObjOpt::WXDelete()
{
   if(0!=mpWXFieldOption)
   {
      VFN_DEBUG_MESSAGE("RefObjOpt::WXDelete()",5)
      delete mpWXFieldOption;
   }
   mpWXFieldOption=0;
}
void RefObjOpt::WXNotifyDelete()
{
   VFN_DEBUG_MESSAGE("RefObjOpt::WXNotifyDelete()",10)
   mpWXFieldOption=0;
}
#endif

//######################################################################
//    RefObjOption
//######################################################################
template<class T> RefObjOption<T>::RefObjOption(T* obj):
mpObj(obj)
{}

template<class T> RefObjOption<T>::~RefObjOption()
{}

template<class T> void RefObjOption<T>::SetChoice(const int choice)
{
   if(mChoice==choice)return;
   VFN_DEBUG_MESSAGE("RefObjOption<T>::SetChoice()"<<this->GetName()<< \
                     " to "<<this->GetChoiceName(choice),5)
   mChoice=choice;
   mClock.Click();
   if(mfpSetNewValue !=0) (mpObj->*mfpSetNewValue)(choice);
}

template<class T> void RefObjOption<T>::Init(const int nbChoice,
                                             const string *name,
                                             const string *choiceNames,
                                             void (T::*fp)(const int))
{
   this->RefObjOpt::Init(nbChoice,name,choiceNames);
   mfpSetNewValue=fp;
}

//######################################################################
//    ObjRegistry
//######################################################################
#ifdef __WX__CRYST__
bool operator==(const wxString&wx,const string&str)
{
   return wx==str.c_str();
}
bool operator==(const string&str,const wxString&wx)
{
   return wx==str.c_str();
}
#endif

template<class T> ObjRegistry<T>::ObjRegistry():
mpRegistry(0),mNbRegistered(0),mMaxNbRegistered(100)
#ifdef __WX__CRYST__
,mpWXRegistry(0)
#endif
{
   VFN_DEBUG_MESSAGE("ObjRegistry::ObjRegistry()",5)
   mpRegistry = new T* [mMaxNbRegistered];
   //this->Print();
}

template<class T> ObjRegistry<T>::ObjRegistry(const string &name):
mpRegistry(0),mNbRegistered(0),mMaxNbRegistered(100),mName(name)
#ifdef __WX__CRYST__
,mpWXRegistry(0)
#endif
{
   VFN_DEBUG_MESSAGE("ObjRegistry::ObjRegistry(name):"<<mName,5)
   mpRegistry = new T* [mMaxNbRegistered];
   //this->Print();
}
//:TODO: a copy constructor
template<class T> ObjRegistry<T>::~ObjRegistry()
{
   VFN_DEBUG_MESSAGE("ObjRegistry::~ObjRegistry():"<<mName,5)
   //this->Print();
   if(mpRegistry!=0) delete[] mpRegistry;
   mpRegistry=0;
   //VFN_DEBUG_MESSAGE("ObjRegistry::~ObjRegistry():End",5)
   #ifdef __WX__CRYST__
   this->WXDelete();
   #endif
}
template<class T> void ObjRegistry<T>::Register(T &obj)
{
   VFN_DEBUG_MESSAGE("ObjRegistry("<<mName<<")::Register():"<<obj.GetName(),2)
   for(unsigned int i=0;i<mNbRegistered;i++) if(mpRegistry[i]==&obj) return;
   if(mNbRegistered==mMaxNbRegistered)
   {
      T** newRegistry = new T* [mMaxNbRegistered+100];
      for(unsigned long i=0;i<mMaxNbRegistered;i++) newRegistry[i]=mpRegistry[i];
      delete[] mpRegistry;
      mpRegistry=newRegistry;
      mMaxNbRegistered+=100;
   }
   mpRegistry[mNbRegistered++]=&obj;
   #ifdef __WX__CRYST__
   if(0!=mpWXRegistry) 
      mpWXRegistry->Add(obj.WXCreate(mpWXRegistry));
   #endif
   //this->Print();
}

template<class T> void ObjRegistry<T>::DeRegister(T &obj)
{
   VFN_DEBUG_MESSAGE("ObjRegistry("<<mName<<")::Deregister(&obj):",2)
   //this->Print();
   const long i=this->Find(obj);
   if(-1==i) return; //:TODO: throw something ?
   #ifdef __WX__CRYST__
   if(0!=mpWXRegistry) mpWXRegistry->Remove(obj.WXGet());
   #endif
   if(i==(long)mNbRegistered) mNbRegistered--;
   else mpRegistry[i]=mpRegistry[--mNbRegistered];
}

template<class T> void ObjRegistry<T>::DeRegister(const string &objName)
{
   VFN_DEBUG_MESSAGE("ObjRegistry("<<mName<<")::Deregister(name):",2)
   const long i=this->Find(objName);
   if(-1==i) return; //:TODO: throw something ?
   #ifdef __WX__CRYST__
   if(0!=mpWXRegistry) mpWXRegistry->Remove(mpRegistry[i]->WXGet());
   #endif
   if(i==((long)mNbRegistered)-1) mNbRegistered--;
   else mpRegistry[i]=mpRegistry[--mNbRegistered];
}

template<class T> void ObjRegistry<T>::DeleteAll()
{
   VFN_DEBUG_MESSAGE("ObjRegistry("<<mName<<")::DeleteAll():",5)
   for(unsigned long i=0;i<mNbRegistered;i++)
   {
      #ifdef __WX__CRYST__
      if(0!=mpWXRegistry) mpWXRegistry->Remove(mpRegistry[i]->WXGet());
      #endif
      delete mpRegistry[i];
   }
   mNbRegistered=0;
}

template<class T> T& ObjRegistry<T>::GetObj(const unsigned int i)
{
   return *mpRegistry[i];
}

template<class T> const T& ObjRegistry<T>::GetObj(const unsigned int i) const
{
   return *mpRegistry[i];
}

template<class T> T& ObjRegistry<T>::GetObj(const string &objName)
{
   const long i=this->Find(objName);
   return *mpRegistry[i];
}

template<class T> const T& ObjRegistry<T>::GetObj(const string &objName) const
{
   const long i=this->Find(objName);
   return *mpRegistry[i];
}

template<class T> T& ObjRegistry<T>::GetObj(const string &objName,
                                                  const string& className)
{
   const long i=this->Find(objName,className);
   return *mpRegistry[i];
}

template<class T> const T& ObjRegistry<T>::GetObj(const string &objName,
                                                        const string& className) const
{
   const long i=this->Find(objName,className);
   return *mpRegistry[i];
}

template<class T> long ObjRegistry<T>::GetNb()const{return (long)mNbRegistered;}

template<class T> void ObjRegistry<T>::Print()const
{
   VFN_DEBUG_MESSAGE("ObjRegistry::Print():",2)
   cout <<mName<<" :"<<mNbRegistered<<" object registered:" <<endl;
   for(unsigned int i=0;i<mNbRegistered;i++) 
      cout <<i<<"("<<mpRegistry[i]->GetName()<<")"<<endl;
}

template<class T> void ObjRegistry<T>::SetName(const string &name){ mName=name;}

template<class T> const string& ObjRegistry<T>::GetName()const { return mName;}

template<class T> long ObjRegistry<T>::Find(const string &objName) const
{
   VFN_DEBUG_MESSAGE("ObjRegistry::Find(objName)",2)
   long index=-1;
   //bool error=false;
   for(long i=this->GetNb()-1;i>=0;i--) 
      if( mpRegistry[i]->GetName() == objName) return i;
   //      if(-1 != index) error=true ;else index=i;
   //if(true == error)
   //{
   //   cout << "ObjRegistry::Find(name) : ";
   //   cout << "found duplicate name ! This *cannot* be !!" ;
   //   cout << objName <<endl;
   //   this->Print();
   //   throw 0;
   //}
   cout << "ObjRegistry<T>::Find("<<objName<<"): Not found !!"<<endl;
   this->Print();
   throw ObjCrystException("ObjRegistry<T>::Find("+objName+"): Not found !!");
   return index;
}

template<class T> long ObjRegistry<T>::Find(const string &objName,
                                               const string &className) const
{
   VFN_DEBUG_MESSAGE("ObjRegistry::Find(objName,className)",2)
   long index=-1;
   //bool error=false;
   for(long i=this->GetNb()-1;i>=0;i--) 
      if( mpRegistry[i]->GetName() == objName) 
         if(className==mpRegistry[i]->GetClassName()) return i;
   //      if(-1 != index) error=true ;else index=i;
   //if(true == error)
   //{
   //   cout << "ObjRegistry::Find(name) : ";
   //   cout << "found duplicate name ! This *cannot* be !!" ;
   //   cout << objName <<endl;
   //   this->Print();
   //   throw 0;
   //}
   cout << "ObjRegistry<T>::Find("<<objName<<","<<className<<"): Not found !!"<<endl;
   this->Print();
   throw 0;
   return index;
}

template<class T> long ObjRegistry<T>::Find(const T &obj) const
{
   VFN_DEBUG_MESSAGE("ObjRegistry::Find(&obj)",2)
   for(long i=this->GetNb()-1;i>=0;i--) 
      if( mpRegistry[i]== &obj)  return i;
   //:TODO: throw something
   return -1;
}
#ifdef __WX__CRYST__
template<class T> WXRegistry<T>* ObjRegistry<T>::WXCreate(wxWindow *parent)
{
   VFN_DEBUG_MESSAGE("ObjRegistry<T>::WXCreate()",2)
   mpWXRegistry=new WXRegistry<T> (parent,this);
   for(int i=0;i<this->GetNb();i++) 
      mpWXRegistry->Add(this->GetObj(i).WXCreate(mpWXRegistry));
   return mpWXRegistry;
}
template<class T> void ObjRegistry<T>::WXDelete()
{
   if(0!=mpWXRegistry)
   {
      VFN_DEBUG_MESSAGE("ObjRegistry<T>::WXDelete()",2)
      delete mpWXRegistry;
   }
   mpWXRegistry=0;
}
template<class T> void ObjRegistry<T>::WXNotifyDelete()
{
   VFN_DEBUG_MESSAGE("ObjRegistry<T>::WXNotifyDelete()",2)
   mpWXRegistry=0;
}
#endif

//######################################################################
//    function RefObjRegisterRecursive
//######################################################################
template<class T> void RefObjRegisterRecursive(T &obj,ObjRegistry<T> &reg)
{
   VFN_DEBUG_MESSAGE("RefObjRegisterRecursive()",3)
   reg.Register(obj);
   ObjRegistry<T> *pObjReg=&(obj.GetSubObjRegistry());
   for(int i=0;i<pObjReg->GetNb();i++)
      RefObjRegisterRecursive(pObjReg->GetObj(i),reg);
   return;
}


//######################################################################
//    RefinableObj
//######################################################################

ObjRegistry<RefinableObj> gRefinableObjRegistry("Global RefinableObj registry");
ObjRegistry<RefinableObj> gTopRefinableObjRegistry("Global Top RefinableObj registry");

RefinableObj::RefinableObj():
mName(""),mNbRefPar(0),mMaxNbRefPar(100),mSavedValuesSetIsUsed(mMaxNbSavedSets),
mNbRefParNotFixed(-1),mIsbeingRefined(false),mDeleteRefParInDestructor(true)
#ifdef __WX__CRYST__
,mpWXCrystObj(0)
#endif
{
   VFN_DEBUG_MESSAGE("RefinableObj::RefinableObj()",3)
   mpRefPar = new RefinablePar*[mMaxNbRefPar];
   mpSavedValuesSet = new CrystVector_double* [mMaxNbSavedSets];
   mpSavedValuesSetName = new string* [mMaxNbSavedSets];
   mSavedValuesSetIsUsed=false;
   gRefinableObjRegistry.Register(*this);
   mSubObjRegistry.SetName("Registry for sub-objects");
   mClientObjRegistry.SetName("Registry for Clients");
   
   VFN_DEBUG_MESSAGE("RefinableObj::RefinableObj():End",2)
}
/*
RefinableObj::RefinableObj(const RefinableObj &old):
mName(old.mName),mMaxNbRefPar(old.mMaxNbRefPar),mSavedValuesSetIsUsed(mMaxNbSavedSets),
mIsbeingRefined(false),mDeleteRefParInDestructor(true)
#ifdef __WX__CRYST__
,mpWXCrystObj(0)
#endif
{
   VFN_DEBUG_MESSAGE("RefinableObj::RefinableObj(RefinableObj&)",3)
   mpRefPar = new RefinablePar*[mMaxNbRefPar];
   mpSavedValuesSet = new CrystVector_double* [mMaxNbSavedSets];
   mpSavedValuesSetName = new string* [mMaxNbSavedSets];
   mSavedValuesSetIsUsed=false;
   *this=old;
   mSubObjRegistry.SetName("Registry for sub-objects of "+mName);
   mClientObjRegistry.SetName("Registry for Clients of "+mName);
   gRefinableObjRegistry.Register(*this);
}
*/
RefinableObj::~RefinableObj()
{
   VFN_DEBUG_MESSAGE("RefinableObj::~RefinableObj():"<<this->GetName(),5)
   if(mpRefPar!=0)
   {
		if(true==mDeleteRefParInDestructor)
      	for(int i=0;i<this->GetNbPar();i++) delete mpRefPar[i];
      delete[] mpRefPar;
   }
   for(long i=0;i<mMaxNbSavedSets;i++)
      if(true==mSavedValuesSetIsUsed(i))
      {
         delete *(mpSavedValuesSetName+i);
         delete *(mpSavedValuesSet+i);
      }
   delete[] mpSavedValuesSet;
   delete[] mpSavedValuesSetName;
   gRefinableObjRegistry.DeRegister(*this);
   for(int i=0;i<mSubObjRegistry.GetNb();i++)
      mSubObjRegistry.GetObj(i).DeRegisterClient(*this);
   VFN_DEBUG_MESSAGE("RefinableObj::~RefinableObj():End",4)
   #ifdef __WX__CRYST__
   this->WXDelete();
   #endif
}

const string RefinableObj::GetClassName() const {return "RefinableObj";}

const string& RefinableObj::GetName() const {return mName;}
void RefinableObj::SetName(const string &name)
{
   VFN_DEBUG_MESSAGE("RefinableObj::SetName()to :"<<name,6)
   mName=name;
   mSubObjRegistry.SetName("Registry for sub-objects of "+mName);
}
/*
void RefinableObj::operator=(const RefinableObj &old)
{
   VFN_DEBUG_MESSAGE("RefinableObj::operator=(RefinableObj&)",3)
   this->ResetParList();
   //this->AddPar(old);
   // Do not copy old saved sets
   //... but erase any that may be stored
   for(long i=0;i<mMaxNbSavedSets;i++)
      if(true==mSavedValuesSetIsUsed(i))
      {
         delete *(mpSavedValuesSetName+i);
         delete *(mpSavedValuesSet+i);
      }
   mSavedValuesSetIsUsed=false;
}
*/
void RefinableObj::PrepareForRefinement() const
{
   VFN_DEBUG_MESSAGE("RefinableObj::PrepareForRefinement()",5)
   mNbRefParNotFixed=0;
   mRefparNotFixedIndex.resize(this->GetNbPar());
   for(long i=0;i<this->GetNbPar();i++)
      if ( (this->GetPar(i).IsFixed() == false) && (this->GetPar(i).IsUsed() == true))
      {
         mRefparNotFixedIndex(mNbRefParNotFixed) = i;
         mNbRefParNotFixed++;
      }
   //this->Print();
   VFN_DEBUG_MESSAGE("RefinableObj::PrepareForRefinement():End",5)
}

void RefinableObj::FixAllPar()
{
   for(long i=0;i<this->GetNbPar();i++) this->GetPar(i).SetIsFixed(true);
   for(int i=0;i<this->GetSubObjRegistry().GetNb();i++)
      this->GetSubObjRegistry().GetObj(i).FixAllPar();
}

void RefinableObj::UnFixAllPar()
{
   for(long i=0;i<this->GetNbPar();i++) this->GetPar(i).SetIsFixed(false);
   for(int i=0;i<this->GetSubObjRegistry().GetNb();i++)
      this->GetSubObjRegistry().GetObj(i).UnFixAllPar();
}

void RefinableObj::SetParIsFixed(const long parIndex,const bool fix)
{
   this->GetPar(parIndex).SetIsFixed(fix);
}

void RefinableObj::SetParIsFixed(const string& parName,const bool fix)
{
   long index=this->FindPar(parName);
   if(-1==index) return; //:TODO:
   this->GetPar(index).SetIsFixed(fix);
}

void RefinableObj::SetParIsFixed(const RefParType *type,const bool fix)
{
   for(long i=0;i<this->GetNbPar();i++) 
      if( this->GetPar(i).GetType()->IsDescendantFromOrSameAs(type))
      {
         //cout << " Fixing ..." << this->GetPar(i).Name()<<endl;
         this->GetPar(i).SetIsFixed(fix);
      }
   for(int i=0;i<this->GetSubObjRegistry().GetNb();i++)
      this->GetSubObjRegistry().GetObj(i).SetParIsFixed(type,fix);
}

void RefinableObj::SetParIsUsed(const string& parName,const bool use)
{
   long i=this->FindPar(parName);
   if(-1==i) return; //:TODO:
   this->GetPar(i).SetIsUsed(use);
}

void RefinableObj::SetParIsUsed(const RefParType *type,const bool use)
{
   for(long i=0;i<this->GetNbPar();i++) 
      if( this->GetPar(i).GetType()->IsDescendantFromOrSameAs(type))
      {
         //cout << " Now used (Waow!) : ..." << this->GetPar(i).Name()<<endl;
         this->GetPar(i).SetIsUsed(use);
      }
   for(int i=0;i<this->GetSubObjRegistry().GetNb();i++)
      this->GetSubObjRegistry().GetObj(i).SetParIsUsed(type,use);
}

long RefinableObj::GetNbPar()const { return mNbRefPar;}

long RefinableObj::GetNbParNotFixed()const {return mNbRefParNotFixed;}

RefinablePar& RefinableObj::GetPar(const long i)
{
   return *mpRefPar[i];
}

const RefinablePar& RefinableObj::GetPar(const long i) const
{
   return *mpRefPar[i];
}

RefinablePar& RefinableObj::GetPar(const string & name)
{
   const long i=this->FindPar(name);
   if(-1==i)
   {
      cout << "RefinableObj::GetPar(name):Cannot find parameter :"<<name<<endl;
      this->Print();
      RefinablePar *p=0;
      return *p; //:KLUDGE: !
      //throw 0;
   }
   return *mpRefPar[i];
}

const RefinablePar& RefinableObj::GetPar(const string & name) const
{
   const long i=this->FindPar(name);
   if(-1==i)
   {
      cout << "RefinableObj::GetPar(name):Cannot find parameter :"<<name<<endl;
      this->Print();
      RefinablePar *p=0;
      return *p; //:KLUDGE: !
      //throw 0;
   }
   return *mpRefPar[i];
}

RefinablePar& RefinableObj::GetPar(const double *p)
{
   const long i=this->FindPar(p);
   if(-1==i)
   {
      cout << "RefinableObj::GetPar(*f):Cannot find parameter :"<<p<<endl;
      this->Print();
      RefinablePar *p=0;
      return *p; //:KLUDGE: !
      //throw 0;
   }
   return *mpRefPar[i];
}

const RefinablePar& RefinableObj::GetPar(const double *p) const
{
   const long i=this->FindPar(p);
   if(-1==i)
   {
      cout << "RefinableObj::GetPar(*f):Cannot find parameter :"<<p<<endl;
      this->Print();
      RefinablePar *p=0;
      return *p; //:KLUDGE: !
      //throw 0;
   }
   return *mpRefPar[i];
}

RefinablePar& RefinableObj::GetParNotFixed(const long i)
{
   return *mpRefPar[mRefparNotFixedIndex(i)];
}

const RefinablePar& RefinableObj::GetParNotFixed(const long i) const
{
   return *mpRefPar[mRefparNotFixedIndex(i)];
}

void RefinableObj::AddPar(const RefinablePar &newRefPar)
{
   VFN_DEBUG_MESSAGE("RefinableObj::AddPar(RefPar&)",2)
   if(mNbRefPar == mMaxNbRefPar)
   {
      mMaxNbRefPar+=100;
      RefinablePar** oldmpRefPar=mpRefPar;
      mpRefPar = new RefinablePar*[mMaxNbRefPar];
      for(int i=0;i<this->GetNbPar();i++) mpRefPar[i]=oldmpRefPar[i];
      delete[] oldmpRefPar;
   }
   mpRefPar[mNbRefPar]=new RefinablePar(newRefPar);
   mNbRefPar++;
}

void RefinableObj::AddPar(RefinablePar *newRefPar)
{
   VFN_DEBUG_MESSAGE("RefinableObj::AddPar(RefPar&)",2)
   if(mNbRefPar == mMaxNbRefPar)
   {
      mMaxNbRefPar+=100;
      RefinablePar** oldmpRefPar=mpRefPar;
      mpRefPar = new RefinablePar*[mMaxNbRefPar];
      for(int i=0;i<this->GetNbPar();i++) mpRefPar[i]=oldmpRefPar[i];
      delete[] oldmpRefPar;
   }
   mpRefPar[mNbRefPar]=newRefPar;
   mNbRefPar++;
}

void RefinableObj::AddPar(RefinableObj &newRefParList)
{
   VFN_DEBUG_MESSAGE("RefinableObj::AddPar(RefParList&)" <<newRefParList.GetNbPar() ,2)
   for(long i=0;i<newRefParList.GetNbPar();i++) this->AddPar(&(newRefParList.GetPar(i)));
}

void RefinableObj::Print() const
{
   VFN_DEBUG_ENTRY("RefinableObj::Print()",2)
   cout << "Refinable Object:"<<this->GetName()
        <<", with " << this->GetNbPar() << " parameters" <<endl;
   for(int i=0;i<this->GetNbPar();i++)
   {
      if(this->GetPar(i).IsUsed() == false) continue;
      cout << "#"<<i<<"#" << this->GetPar(i).GetName() << ": " ;
      cout << FormatFloat(this->GetPar(i).GetHumanValue(),18,12) << " ";
      if(true == this->GetPar(i).IsFixed()) cout << "Fixed";
      else 
         if(true == this->GetPar(i).IsLimited())
         { 
            cout << "Limited (" << this->GetPar(i).GetHumanMin()<<","
                 <<this->GetPar(i).GetHumanMax()<<")";
            if(true == this->GetPar(i).IsPeriodic()) cout << ",Periodic" ;
         }
      VFN_DEBUG_MESSAGE_SHORT(" (at "<<this->GetPar(i).mValue<<")",5)
      if(true == this->GetPar(i).mHasAssignedClock)
         VFN_DEBUG_MESSAGE_SHORT(" (Clock at "<<this->GetPar(i).mpClock<<")",5)
      cout << endl;
   }
   VFN_DEBUG_EXIT("RefinableObj::Print()",2)
}

long RefinableObj::CreateParamSet(const string name) const
{
   VFN_DEBUG_MESSAGE("RefinableObj::CreateRefParSet()",3)
   long id;
   for(id=0;id<mMaxNbSavedSets;id++) if(false==mSavedValuesSetIsUsed(id)) break;
   if(mMaxNbSavedSets==id)
   {//:TODO:
      cout << "RefinableObj::CreateRefParSet():cannot store that many saved sets !"<<endl;
      throw 0;
   }
   mSavedValuesSetIsUsed(id)=true;
   *(mpSavedValuesSetName+id)= new string;
   **(mpSavedValuesSetName+id) = name;
   *(mpSavedValuesSet+id)=new CrystVector_double;
   (*(mpSavedValuesSet+id))->resize(mNbRefPar);
   double *p=(*(mpSavedValuesSet+id))->data();
   for(long i=0;i<mNbRefPar;i++) *p++ = this->GetPar(i).GetValue();
   return id;
}

void RefinableObj::SaveParamSet(const long id)const
{
   VFN_DEBUG_MESSAGE("RefinableObj::SaveRefParSet()",2)
   if(true != mSavedValuesSetIsUsed(id))
   {//throw up
      cout << "RefinableObj::SaveRefParSet(long): Unknown saved set !" <<endl;
      throw 0;//:TODO: some more inteligent exception
   }
   (*(mpSavedValuesSet+id))->resize(mNbRefPar);
   double *p=(*(mpSavedValuesSet+id))->data();
   for(long i=0;i<mNbRefPar;i++) *p++ = this->GetPar(i).GetValue();
}

void RefinableObj::RestoreParamSet(const long id)
{
   VFN_DEBUG_MESSAGE("RefinableObj::RestoreRefParSet()",2)
   const double *p=(*(mpSavedValuesSet+id))->data();
   for(long i=0;i<mNbRefPar;i++)
   {
      if( !this->GetPar(i).IsFixed() && this->GetPar(i).IsUsed())
         this->GetPar(i).SetValue(*p);
      p++;
   }
}

const CrystVector_double & RefinableObj::GetParamSet(const long id)const
{
   VFN_DEBUG_MESSAGE("RefinableObj::RefParSet()",0)
   return **(mpSavedValuesSet+id);
}

double RefinableObj::GetParamSet_ParNotFixedHumanValue(const long id,
                                                      const long par)const
{
   VFN_DEBUG_MESSAGE("RefinableObj::RefParSetNotFixedHumanValue()",0)
   return (**(mpSavedValuesSet+id))(mRefparNotFixedIndex(par));
}

const void RefinableObj::EraseAllParamSet()
{
   for(long i=0;i<mMaxNbSavedSets;i++)
      if(true==mSavedValuesSetIsUsed(i))
      {
         delete *(mpSavedValuesSetName+i);
         delete *(mpSavedValuesSet+i);
      }
   mSavedValuesSetIsUsed=false;
}

void RefinableObj::SetLimitsAbsolute(const string &name,const double min,const double max)
{
   const long i=this->FindPar(name);
   this->GetPar(i).SetLimitsAbsolute(min,max);
}
void RefinableObj::SetLimitsAbsolute(const RefParType *type,
                                     const double min,const double max)
{
   for(long i=0;i<mNbRefPar;i++)
      if(this->GetPar(i).GetType()->IsDescendantFromOrSameAs(type))
         this->GetPar(i).SetLimitsAbsolute(min,max);
   for(int i=0;i<this->GetSubObjRegistry().GetNb();i++)
      this->GetSubObjRegistry().GetObj(i).SetLimitsAbsolute(type,min,max);
}
void RefinableObj::SetLimitsRelative(const string &name, const double min, const double max)
{
   const long i=this->FindPar(name);
   this->GetPar(i).SetLimitsRelative(min,max);
}
void RefinableObj::SetLimitsRelative(const RefParType *type,
                                     const double min, const double max)
{
   VFN_DEBUG_MESSAGE("RefinableObj::SetLimitsRelative(RefParType*):"<<this->GetName(),2)
   for(long i=0;i<mNbRefPar;i++)
   {
   	VFN_DEBUG_MESSAGE("RefinableObj::SetLimitsRelative(RefParType*):par #"<<i,2)
      if(this->GetPar(i).GetType()->IsDescendantFromOrSameAs(type))
         this->GetPar(i).SetLimitsRelative(min,max);
   }
   for(int i=0;i<this->GetSubObjRegistry().GetNb();i++)
      this->GetSubObjRegistry().GetObj(i).SetLimitsRelative(type,min,max);
}
void RefinableObj::SetLimitsProportional(const string &name,const double min,const double max)
{
   const long i=this->FindPar(name);
   this->GetPar(i).SetLimitsProportional(min,max);
}
void RefinableObj::SetLimitsProportional(const RefParType *type, 
                                         const double min, const double max)
{
   for(long i=0;i<mNbRefPar;i++)
      if(this->GetPar(i).GetType()->IsDescendantFromOrSameAs(type)) 
         this->GetPar(i).SetLimitsProportional(min,max);
   for(int i=0;i<this->GetSubObjRegistry().GetNb();i++)
      this->GetSubObjRegistry().GetObj(i).SetLimitsProportional(type,min,max);
}

ObjRegistry<RefinableObj>& RefinableObj::GetSubObjRegistry()
{return mSubObjRegistry;}

const ObjRegistry<RefinableObj>& RefinableObj::GetSubObjRegistry()const 
{return mSubObjRegistry;}

void RefinableObj::RegisterClient(RefinableObj &obj)const 
{mClientObjRegistry.Register(obj);}

void RefinableObj::DeRegisterClient(RefinableObj &obj)const
{mClientObjRegistry.DeRegister(obj);}

bool RefinableObj::IsBeingRefined()const {return mIsbeingRefined;}

void RefinableObj::BeginOptimization()
{
   mIsbeingRefined=true;
   this->Prepare();
   for(int i=0;i<mSubObjRegistry.GetNb();i++)
      mSubObjRegistry.GetObj(i).BeginOptimization();
   #ifdef __WX__CRYST__
   if(0!=mpWXCrystObj) mpWXCrystObj->Enable(false);
   #endif
}

void RefinableObj::EndOptimization()
{
   mIsbeingRefined=false;
   this->UpdateDisplay();
   for(int i=0;i<mSubObjRegistry.GetNb();i++)
      mSubObjRegistry.GetObj(i).EndOptimization();
   #ifdef __WX__CRYST__
   if(0!=mpWXCrystObj) mpWXCrystObj->Enable(true);
   #endif
}

void RefinableObj::RandomizeConfiguration()
{
   VFN_DEBUG_ENTRY("RefinableObj::RandomizeConfiguration():"<<mName,5)
   static bool need_initRandomSeed=true;
   if(need_initRandomSeed==true)
   {
      this->InitRandomSeedFromTime();
      need_initRandomSeed=false;
   }
   this->PrepareForRefinement();
   for(int j=0;j<this->GetNbParNotFixed();j++)
   {
      if(true==this->GetParNotFixed(j).IsLimited())
      {
         const double min=this->GetParNotFixed(j).GetMin();
         const double max=this->GetParNotFixed(j).GetMax();
         this->GetParNotFixed(j).MutateTo(min+(max-min)*(rand()/(double)RAND_MAX) );
      }
		else
      	if(true==this->GetParNotFixed(j).IsPeriodic())
			{

         	this->GetParNotFixed(j).MutateTo((rand()/(double)RAND_MAX)
						* this->GetParNotFixed(j).GetPeriod());
			}
   }
   for(int i=0;i<this->GetSubObjRegistry().GetNb();i++)
      this->GetSubObjRegistry().GetObj(i).RandomizeConfiguration();
   VFN_DEBUG_EXIT("RefinableObj::RandomizeConfiguration():Finished",5)
}

void RefinableObj::GlobalOptRandomMove(const double mutationAmplitude)
{
   VFN_DEBUG_MESSAGE("RefinableObj::GlobalOptRandomMove()",2)
   for(int j=0;j<this->GetNbParNotFixed();j++)
   {
      this->GetParNotFixed(j).Mutate( this->GetParNotFixed(j).GetGlobalOptimStep()
                  *2*(rand()/(double)RAND_MAX-0.5)*mutationAmplitude);
   }
   for(int i=0;i<mSubObjRegistry.GetNb();i++)
      mSubObjRegistry.GetObj(i).GlobalOptRandomMove(mutationAmplitude);
   VFN_DEBUG_MESSAGE("RefinableObj::GlobalOptRandomMove():End",1)
}

unsigned int RefinableObj::GetNbCostFunction()const {return 0;}

const string& RefinableObj::GetCostFunctionName(const unsigned int id)const
{
   cout << "RefinableObj::GetCostFunctionName(): Not Found !" <<endl;
   throw 0;
}

const string& RefinableObj::GetCostFunctionDescription(const unsigned int id)const
{
   cout << "RefinableObj::GetCostFunctionDescription(): Not Found !" <<endl;
   throw 0;
}

double RefinableObj::GetCostFunctionValue(const unsigned int)
{
   cout << "RefinableObj::GetCostFunctionValue(): no cost functions !" <<endl;
   throw 0;
}

void RefinableObj::ResetParList()
{
   VFN_DEBUG_MESSAGE("RefinableObj::ResetParList()",3)
   if(mpRefPar!=0)
   {
		if(true==mDeleteRefParInDestructor)
      	for(int i=0;i<this->GetNbPar();i++) delete mpRefPar[i];
      delete[] mpRefPar;
   }
   mpRefPar = new RefinablePar*[mMaxNbRefPar];
   mNbRefPar=0;
   mNbRefParNotFixed=-1;

   VFN_DEBUG_MESSAGE("RefinableObj::ResetParList():Deleting Saved Sets....",2)
   for(long i=0;i<mMaxNbSavedSets;i++)
      if(true==mSavedValuesSetIsUsed(i))
      {
   VFN_DEBUG_MESSAGE("RefinableObj::ResetParList():Deleting Saved Set "<<i,2)
         delete *(mpSavedValuesSetName+i);
         delete *(mpSavedValuesSet+i);
      }
   mSavedValuesSetIsUsed=false;
   VFN_DEBUG_MESSAGE("RefinableObj::ResetParList():End.",3)
}

unsigned int RefinableObj::GetNbOption()const
{
   return mOptionRegistry.GetNb();
}

RefObjOpt& RefinableObj::GetOption(const unsigned int i)
{
   VFN_DEBUG_MESSAGE("RefinableObj::GetOption()"<<i,3)
   //:TODO: Check
   return mOptionRegistry.GetObj(i);
}

void RefinableObj::GetGeneGroup(const RefinableObj &obj,
														  CrystVector_uint & groupIndex,
											           unsigned int &first) const
{
   VFN_DEBUG_MESSAGE("RefinableObj::GetGeneGroup()",4)
	for(long i=0;i<obj.GetNbPar();i++)
		for(long j=0;j<this->GetNbPar();j++)
			if(obj.mpRefPar[i]->mValue == mpRefPar[j]->mValue) groupIndex(i)= first++;
}
void RefinableObj::SetDeleteRefParInDestructor(const bool b) {mDeleteRefParInDestructor=b;}

const RefObjOpt& RefinableObj::GetOption(const unsigned int i)const
{
   //:TODO: Check
   return mOptionRegistry.GetObj(i);
}

void RefinableObj::UpdateDisplay()const
{
   #ifdef __WX__CRYST__
   VFN_DEBUG_ENTRY("RefinableObj::UpdateDisplay()",3)
      if(0!=mpWXCrystObj) mpWXCrystObj->CrystUpdate();;
   VFN_DEBUG_EXIT("RefinableObj::UpdateDisplay()",3)
   #endif
}

long RefinableObj::FindPar(const string &name) const
{
   long index=-1;
   bool warning=false;
   for(long i=this->GetNbPar()-1;i>=0;i--) 
      if( this->GetPar(i).GetName() == name) 
         if(-1 != index) warning=true ;else index=i;
   if(true == warning)
   {
      cout << "RefinableObj::FindPar(name) : ";
      cout << "found duplicate refinable variable name ! This *cannot* be !!" ;
      cout << name <<endl;
      throw 0;
   }
   
   //if(index == -1)
   //{
   //   cout << "RefinableObj::FindPar(name) :";
   //   cout << "Cannot find refinable variable name :#"<< name << "#"<<endl;
   //   this->Print();
   //   //throw 0;
   //}
   return index;
}

long RefinableObj::FindPar(const double *p) const
{
   long index=-1;
   bool warning=false;
   for(long i=this->GetNbPar()-1;i>=0;i--) 
      if( p == mpRefPar[i]->mValue ) //&(this->GetPar(i).GetValue())
         if(-1 != index) warning=true ;else index=i;
   if(true == warning)
   {
      cout << "RefinableObj::FindPar(*f) : ";
      cout << "found duplicate refinable variable name ! This *cannot* be !!" ;
      cout << p <<endl;
      //throw 0;
   }
   
   //if(index == -1)
   //{
   //   cout << "RefinableObj::FindPar(name) :";
   //   cout << "Cannot find refinable variable name :#"<< name << "#"<<endl;
   //   this->Print();
   //   //throw 0;
   //}
   return index;
}

void RefinableObj::AddSubRefObj(RefinableObj &obj)
{
   mSubObjRegistry.Register(obj);
}

void RefinableObj::RemoveSubRefObj(RefinableObj &obj)
{
   mSubObjRegistry.DeRegister(obj);
}

void RefinableObj::InitRandomSeedFromTime()const
{
   VFN_DEBUG_MESSAGE("RefinableObj::InitRandomSeedFromTime()",3)
   time_t junk;
   time(&junk);
   tm *tmp=localtime(&junk);
   srand((unsigned)( (*tmp).tm_sec+60* (*tmp).tm_min));
}

void RefinableObj::AddOption(RefObjOpt *opt)
{
   VFN_DEBUG_MESSAGE("RefinableObj::AddOption()",5)
   //:TODO: automagically resize the option array if necessary
   mOptionRegistry.Register(*opt);
   VFN_DEBUG_MESSAGE("RefinableObj::AddOption():End",5)
}

void RefinableObj::Prepare()
{
   VFN_DEBUG_MESSAGE("RefinableObj::Prepare()",5)
   for(int i=0;i<this->GetSubObjRegistry().GetNb();i++)
      this->GetSubObjRegistry().GetObj(i).Prepare();
}

#ifdef __WX__CRYST__
WXCrystObjBasic* RefinableObj::WXCreate(wxWindow *parent)
{
   VFN_DEBUG_MESSAGE("RefinableObj::WXCreate()",10)
   mpWXCrystObj=new WXRefinableObj (parent,this);
   return mpWXCrystObj;
}
WXCrystObjBasic* RefinableObj::WXGet()
{
   return mpWXCrystObj;
}
void RefinableObj::WXDelete()
{
   if(0!=mpWXCrystObj)
   {
      VFN_DEBUG_MESSAGE("RefinableObj::WXDelete()",5)
      delete mpWXCrystObj;
   }
   mpWXCrystObj=0;
}
void RefinableObj::WXNotifyDelete()
{
   VFN_DEBUG_MESSAGE("RefinableObj::WXNotifyDelete():"<<mName,5)
   mpWXCrystObj=0;
}
#endif

//***********EXPLICIT INSTANTIATION*******************//
template void RefObjRegisterRecursive(RefinableObj &obj,ObjRegistry<RefinableObj> &reg);
}//namespace
#include "ObjCryst/Crystal.h"
#include "ObjCryst/Scatterer.h"
#include "ObjCryst/ScatteringPower.h"
#include "ObjCryst/ZScatterer.h"
#include "ObjCryst/PowderPattern.h"
#include "ObjCryst/DiffractionDataSingleCrystal.h"
#include "RefinableObj/GlobalOptimObj.h"
#include "RefinableObj/IO.h"
using namespace ObjCryst;
template class ObjRegistry<RefObjOpt>;
template class ObjRegistry<RefinableObj>;
template class ObjRegistry<Crystal>;
template class ObjRegistry<Scatterer>;
template class ObjRegistry<ScatteringPower>;
template class ObjRegistry<ScatteringPowerAtom>;
template class ObjRegistry<PowderPattern>;
template class ObjRegistry<PowderPatternComponent>;
template class ObjRegistry<DiffractionDataSingleCrystal>;
template class ObjRegistry<OptimizationObj>;
//template class ObjRegistry<IOCrystTag>;//to be removed
template class ObjRegistry<XMLCrystTag>;
template class ObjRegistry<ZAtom>;

template class RefObjOption<RefinableObj>;
template class RefObjOption<Crystal>;
template class RefObjOption<Radiation>;
template class RefObjOption<Scatterer>;
template class RefObjOption<ScatteringPower>;
template class RefObjOption<ScatteringPowerAtom>;
template class RefObjOption<PowderPattern>;
template class RefObjOption<PowderPatternComponent>;
template class RefObjOption<PowderPatternBackground>;
template class RefObjOption<DiffractionDataSingleCrystal>;
template class RefObjOption<PowderPatternDiffraction>;
//template class RefObjOption<GlobalOptimObj>;
//template class RefObjOption<IOCrystTag>;

