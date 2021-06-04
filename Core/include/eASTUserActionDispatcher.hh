// ********************************************************************
//
// eASTUserActionDispatcher.hh
//   Header file to store component-specific user actions
//   Please note that this class does not handle the following actions
//     - G4UserPrimaryGeneratorAction
//     - G4UserSteppingAction
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTUserActionDispatcher_H 
#define eASTUserActionDispatcher_H 1

class G4Region;
class G4UserRunAction;
class G4UserEventAction;
class G4UserStackingAction;
class G4UserTrackingAction;

#include <map>
#include "G4Threading.hh"

class eASTUserActionDispatcher
{
  public:
    using RunActions = std::map<G4Region*,G4UserRunAction*>;
    using EventActions = std::map<G4Region*,G4UserEventAction*>;
    using StackingActions = std::map<G4Region*,G4UserStackingAction*>;
    using TrackingActions = std::map<G4Region*,G4UserTrackingAction*>;

  public:
    static eASTUserActionDispatcher* Instance();

  private:
    static G4ThreadLocal eASTUserActionDispatcher* instance;

  private:
    eASTUserActionDispatcher()
    {;}

  public:
    ~eASTUserActionDispatcher();

  public:
    RunActions* GetRunActions() 
    { return fpRunActions; }
    EventActions* GetEventActions()
    { return fpEventActions; }
    StackingActions* GetStackingActions()
    { return fpStackingActions; }
    TrackingActions* GetTrackingActions()
    { return fpTrackingActions; }

  public:
    void RegisterUserAction(G4Region* reg,G4UserRunAction* ua)
    {
      if(fpRunActions==nullptr) fpRunActions = new RunActions;
      (*fpRunActions)[reg] = ua;
    }
    void RegisterUserAction(G4Region* reg,G4UserEventAction* ua)
    { 
      if(fpEventActions==nullptr) fpEventActions = new EventActions;
      (*fpEventActions)[reg] = ua; 
    }
    void RegisterUserAction(G4Region* reg,G4UserStackingAction* ua)
    { 
      if(fpStackingActions==nullptr) fpStackingActions = new StackingActions;
      (*fpStackingActions)[reg] = ua; 
    }
    void RegisterUserAction(G4Region* reg,G4UserTrackingAction* ua)
    { 
      if(fpTrackingActions==nullptr) fpTrackingActions = new TrackingActions;
      (*fpTrackingActions)[reg] = ua; 
    }

  private:
    RunActions* fpRunActions = nullptr;
    EventActions* fpEventActions = nullptr;
    StackingActions* fpStackingActions = nullptr;
    TrackingActions* fpTrackingActions = nullptr;
};

#endif

