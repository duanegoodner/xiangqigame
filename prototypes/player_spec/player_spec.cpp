#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>

enum EvaluatorType : int { kSimple = 0, kMinimax = 1 };

enum ZobristKeyType : int { kNoKey = 0, k032 = 1, k064 = 2 };

enum ZobristCalculatorCount : int { kNoZCalcs = 0, kOne = 1, kTwo = 2 };

class EvaluatorBase {
public:
  virtual void SelectItem() = 0;
  virtual ~EvaluatorBase() = default;
};

class FactoryBase {
public:
  virtual std::unique_ptr<EvaluatorBase> Create() = 0;
  virtual ~FactoryBase() = default;
};

//
//

class SimpleEvaluator : public EvaluatorBase {
  std::istream &input_stream_;

public:
  SimpleEvaluator(std::istream &input_stream = std::cin)
      : input_stream_{input_stream} {}
  void SelectItem() { std::cout << "Human is selecting an item" << std::endl; }
};

class SimpleEvaluatorFactory : public FactoryBase {
  std::istream &input_stream_;

public:
  SimpleEvaluatorFactory(std::istream &input_stream = std::cin)
      : input_stream_{input_stream} {}
  std::unique_ptr<EvaluatorBase> Create() {
    return std::make_unique<SimpleEvaluator>(input_stream_);
  }
};

//
//

template <typename KeyType, size_t NumKeys>
class MinimaxEvaluator : public EvaluatorBase {
  size_t search_depth_;

public:
  MinimaxEvaluator(size_t search_depth)
      : search_depth_{search_depth} {}

  void SelectItem() {
    std::cout << "Complex evaluator is selecting an item" << std::endl;
  }
};

template <typename KeyType, size_t NumConfKeys>
class MinimaxEvaluatorFactory : public FactoryBase {
  size_t search_depth_;

public:
  MinimaxEvaluatorFactory(size_t search_depth)
      : search_depth_{search_depth} {}

  std::unique_ptr<EvaluatorBase> Create() {
    return std::make_unique<MinimaxEvaluator<KeyType, NumConfKeys>>(search_depth_);
  }
};

struct MinimaxTypeInfo {
  ZobristKeyType zobrist_key_type;
  ZobristCalculatorCount zobrist_calculator_count;

  MinimaxTypeInfo(
      ZobristKeyType zobrist_key_type = ZobristKeyType::kNoKey,
      ZobristCalculatorCount zobrist_calculator_count = ZobristCalculatorCount::kNoZCalcs
  )
      : zobrist_key_type{zobrist_key_type}
      , zobrist_calculator_count{zobrist_calculator_count} {}

  bool operator==(const MinimaxTypeInfo &other) const {
    return zobrist_key_type == other.zobrist_key_type &&
           zobrist_calculator_count == other.zobrist_calculator_count;
  }
};

struct MinimaxTypeInfoHash {
  std::size_t operator()(const MinimaxTypeInfo &key) const {
    std::size_t h1 = std::hash<int>{}(key.zobrist_key_type);
    std::size_t h2 = std::hash<int>{}(key.zobrist_calculator_count);
    // Combine the two hash values
    return h1 ^ (h2 << 1); // XOR and shift
  }
};

struct PlayerSpec {
  EvaluatorType evaluator_type;
  MinimaxTypeInfo minimax_type_info;
  size_t minimax_search_depth;
  std::istream &input_stream;

  PlayerSpec(
      EvaluatorType evaluator_type = EvaluatorType::kSimple,
      MinimaxTypeInfo minimax_type_info = MinimaxTypeInfo{},
      size_t minimax_search_depth = 0,
      std::istream &input_stream = std::cin
  )
      : evaluator_type{evaluator_type}
      , input_stream{input_stream}
      , minimax_type_info{minimax_type_info}
      , minimax_search_depth{minimax_search_depth} {}
};

class EvaluatorFactoryRetriever {
  std::unordered_map<MinimaxTypeInfo, std::shared_ptr<FactoryBase>, MinimaxTypeInfoHash>
      minimax_factories_;
  const PlayerSpec &evaluator_factory_info;

public:
  EvaluatorFactoryRetriever(const PlayerSpec &evaluator_factory_info)
      : evaluator_factory_info{evaluator_factory_info}
      , minimax_factories_{} {
    minimax_factories_.emplace(
        MinimaxTypeInfo{ZobristKeyType::k032, ZobristCalculatorCount::kOne},
        std::make_shared<MinimaxEvaluatorFactory<uint32_t, 0>>(
            evaluator_factory_info.minimax_search_depth
        )
    );
    minimax_factories_.emplace(
        MinimaxTypeInfo{ZobristKeyType::k064, ZobristCalculatorCount::kOne},
        std::make_shared<MinimaxEvaluatorFactory<uint32_t, 0>>(
            evaluator_factory_info.minimax_search_depth
        )
    );
    minimax_factories_.emplace(
        MinimaxTypeInfo{ZobristKeyType::k032, ZobristCalculatorCount::kTwo},
        std::make_shared<MinimaxEvaluatorFactory<uint32_t, 1>>(
            evaluator_factory_info.minimax_search_depth
        )
    );
    minimax_factories_.emplace(
        MinimaxTypeInfo{ZobristKeyType::k064, ZobristCalculatorCount::kTwo},
        std::make_shared<MinimaxEvaluatorFactory<uint32_t, 1>>(
            evaluator_factory_info.minimax_search_depth
        )
    );
  }

  std::shared_ptr<FactoryBase> GetFactory() {
    std::shared_ptr<FactoryBase> factory;

    if (evaluator_factory_info.evaluator_type == EvaluatorType::kSimple) {
      factory = std::make_shared<SimpleEvaluatorFactory>(evaluator_factory_info.input_stream);
    } else if (evaluator_factory_info.evaluator_type == EvaluatorType::kMinimax) {
      factory = minimax_factories_.at(evaluator_factory_info.minimax_type_info);
    }
    return factory;
  }
};

int main() {

  auto factory_a_info = PlayerSpec(
      EvaluatorType::kMinimax,
      MinimaxTypeInfo{ZobristKeyType::k032, ZobristCalculatorCount::kTwo},
      4
  );
  auto factory_a_retriever =
      EvaluatorFactoryRetriever{factory_a_info};
  auto factory_a = factory_a_retriever.GetFactory();
  auto evaluator_a = factory_a->Create();
  evaluator_a->SelectItem();

  auto factory_b_info = PlayerSpec(EvaluatorType::kSimple);
  auto factory_b_retriever = EvaluatorFactoryRetriever{factory_b_info};
  auto factory_b = factory_b_retriever.GetFactory();
  auto evaluator_b = factory_b->Create();
  evaluator_b->SelectItem();
 

  return 0;
}