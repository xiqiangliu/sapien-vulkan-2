#pragma once
#include "svulkan2/resource/model.h"
#include "transform.h"
#include <memory>
#include <string>
#include <vector>

namespace svulkan2 {
namespace scene {

class Scene;

class Node {
protected:
  std::string mName;
  Transform mTransform{};
  Node *mParent{};
  std::vector<Node *> mChildren{};

  Scene *mScene{nullptr};
  bool mRemoved{false};

public:
  enum Type { eNode, eObject, eCamera, eLight, eUnknown };

  Node(std::string const &name = "");

  virtual inline Type getType() const { return Type::eNode; }

  inline void setName(std::string const &name) { mName = name; }
  inline std::string getName() const { return mName; }

  inline void setScene(Scene *scene) { mScene = scene; }
  inline Scene *getScene() const { return mScene; }

  inline void setParent(Node &parent) { mParent = &parent; }
  inline Node &getParent() const { return *mParent; }

  inline std::vector<Node *> const &getChildren() const { return mChildren; }

  inline void addChild(Node &child) { mChildren.push_back(&child); }
  void removeChild(Node &child);
  void clearChild();

  void markRemovedRecursive();
  inline void markRemoved() { mRemoved = true; }
  inline bool isMarkedRemoved() { return mRemoved; };

  void setTransform(Transform const &transform);
  inline Transform const &getTransform() const { return mTransform; }

  void setPosition(glm::vec3 const &pos);
  void setRotation(glm::quat const &rot);
  void setScale(glm::vec3 const &scale);
  inline glm::vec3 getPosition() const { return mTransform.position; }
  inline glm::quat getRotation() const { return mTransform.rotation; }
  inline glm::vec3 getScale() const { return mTransform.scale; }

  /** called before rendering to update the cached model matrix */
  void updateGlobalModelMatrixRecursive();
  std::vector<class Object *> getObjectsRecursive() const;

  /** force compute current model matrix */
  glm::mat4 computeWorldModelMatrix() const;

  Node(Node const &other) = delete;
  Node &operator=(Node const &other) = delete;
  Node(Node &&other) = delete;
  Node &operator=(Node &&other) = delete;

  virtual ~Node() = default;
};

} // namespace scene
} // namespace svulkan2
