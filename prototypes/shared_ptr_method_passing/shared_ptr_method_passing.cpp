#include <iostream>
#include <memory>
#include <vector>

struct EventObject {
  int start;
  int end;
};

template <typename KeyType>
class EventRecorder {

  KeyType key_;

public:
  EventRecorder()
      : key_{0} {}
  void ProcessEvent(const EventObject &event) { key_++; }

private:
};
  
template <typename KeyTypeA, typename KeyTypeB>
class ActivityCenter {
  std::shared_ptr<EventRecorder<KeyTypeA>> recorder_a_;
  std::shared_ptr<EventRecorder<KeyTypeB>> recorder_b_;

public:
  ActivityCenter(
      std::shared_ptr<EventRecorder<KeyTypeA>> recorder_a,
      std::shared_ptr<EventRecorder<KeyTypeB>> recorder_b
  )
      : recorder_a_{recorder_a}
      , recorder_b_{recorder_b} {}

  void InformRecorders(const EventObject &event) {
    recorder_a_->ProcessEvent(event);
    recorder_b_->ProcessEvent(event);
  }
};

class EventRecorderBuilder {
public:
  template <typename KeyType>
  std::shared_ptr<EventRecorder<KeyType>> build() {
    return std::make_shared<EventRecorder<KeyType>>();
  }
};

class ActivityCenterBuilder {
public:
  template <typename KeyTypeA, typename KeyTypeB>

  std::shared_ptr<ActivityCenter<KeyTypeA, KeyTypeB>> build(
      std::shared_ptr<EventRecorder<KeyTypeA>> recorder_a,
      std::shared_ptr<EventRecorder<KeyTypeB>> recorder_b
  ) {
    return std::make_shared<ActivityCenter<KeyTypeA, KeyTypeB>>(recorder_a, recorder_b);
  }
};

int main() {

  EventRecorderBuilder event_recorder_builder;
  auto recorder_a = event_recorder_builder.build<uint32_t>();
  auto recorder_b = event_recorder_builder.build<uint64_t>();

  ActivityCenterBuilder activity_center_builder;
  auto activity_center =
      activity_center_builder.build<uint32_t, uint64_t>(recorder_a, recorder_b);

  EventObject event{1, 2};

  activity_center->InformRecorders(event);

  return 0;
}