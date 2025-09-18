# Ortha - Client Lib

The core library in Ortha - https://github.com/ortha-ge/core-lib a learning project for exploring the making of
a game-engine using modern C++ including C++20 modules, and a variety of off-the-shelf libraries.

## Source Overview

Ortha makes heavy use of EnTT and is generally divided into different `systems` that will process EnTT views on a 
Scheduler task tick to process entity components. Entities with specific components are gradually transformed through
a chain of systems in order to be usable by final consumer systems. Example:

- Adding `ResourceLoadRequest` with a `ResourceHandle` and factory to an entity will be processed by `ResourceLoadSystem`.
- `ResourceLoadSystem` will check `ResourceCache` to see if a `Resource` has already been loaded for the requested path.
- If not found a new entity with a `Resource` is created and the factory (ie. `SoLoudAudioSource`) is run on the entity. 
A `FileDescriptor` and `FileLoadRequest` are also added to facilitate file loading.
- `FileLoadSystem` will process `FileLoadRequest` and `FileDescriptor` to produce a `RawDataResource` once finished.
- `SoLoudSystem` will process `SoundDescriptor` and `RawDataResource` to produce an internal resource for playing the sound.
- Systems would wait on the expected final resource ie. `SoLoudAudioSource` before processing further.

### root folder

- EnTTRegistry: this is needed as EnTT doesn't play well with C++20 modules due to static internal linkage usage.
    Simply wraps an entt::registry.
- kangaru: this wraps the kangaru DI library (a custom fork). Also has static internal linkage usage so uses a fork.
    The fork is using extern and those are defined in kangaru.cpp
- Scheduler: Allows for scheduling of tasks that are updated when 'tick' is called. schedule returns a TaskHandle
    which will automatically unschedule itself on destruction, or can be manually unscheduled.
- Timer: Keeps track of time and provides an averaged deltaT for use in game logic.

### services

Kangaru services live here although all classes can just as well be constructed manually providing dependencies directly.

### reflection folder

A runtime type-information system used mainly in the serialization system.

  - TypeId: A handle representing the information requested for type T via TypeId::get<T>().
  - TypeTraits: A variant that can be BasicTypeTraits or special use cases ie. MapTypeTraits.
  - BasicTypeTraits: Basic helpers for the type eg. construct factory, destroy, assign.
  - Map/Optional/Variant/VectorTypeTraits: Specialty helpers for the type eg. forEach, get.
  - ReflectionContext: A repository for building name-value pairs for a type ie. Class properties, Enum enumerators.
  - ClassReflection: Class information including it's name, properties, methods. Associated with a given TypeId.
  - EnumReflection: Enum information including it's name and enumerators. Associated with a given TypeId.
  - TypeReflection: Basic type information including a name. Can be used for factory creation of types ie. `float`.
  - reflect<T>: Template specialization for building class/enum/type reflections.
  - Any: Wrapper for TypeId and void*. Can allocate a new instance for a given TypeId or wrap an existing one.

### platform folder

### logging folder

### processor folder

EnTT views can make use of `ProcessError` in entt::exclude in order to stop the processing chain when an error is
encountered such as invalid file path, etc. `ProcessError` can also be used to retry immediately, in a number of frames,
or with a given timepoint to give time for other dependencies to resolve. A dependency that is fully halted should also
chain a `ProcessError` so that all parts of the processing eventually stop trying for a given entity.

### resources folder

### serialization folder

Uses the reflection system to load from data. Can directly call load/save for a type or can use JsonTypeLoader and
adapters on an entity to have the type loaded from Json and attached to the entity once finished.

### node folder

Allows for EnTT entities to be structured in a hierarchy, used for concepts like a game 'scene' or constructing more
complex game objects ie. car with body and wheels, or simply organizing pieces into sub-components. Example:

- Character Root: entt node with spatial, rigidbody, collider, game logic components.
  - Gfx node: entt node with spatial, RenderObject, SpriteObject. Attached materials and sprite resources.
  - Audio node: entt node with spatial, AudioSource, attached sound resources.

- Car Root: entt node with rigid
    - Front wheel: entt node with RenderObjects, etc.
    - Back-wheel:
    - etc.

GlobalSpatial is automatically created for EnTTNode hierarchies in `GlobalSpatialUpdateSystem`, so global position of 
nodes can be used to render in correct positions. Spatial is always local space.

# TODO

- [ ] Test coverage of Any.
- [ ] Enable 'reflect<T>' to resolve to the namespace of 'T'.
- [ ] ImageDescriptor should use JSON for image loading params.
