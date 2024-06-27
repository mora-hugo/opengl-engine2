#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <map>
#include <vector>
#include "File/FileIO.h"
#include "Patterns/Singleton.h"
#include "Chunk/Chunk.h"
#include "Chunk/ChunkManager.h"

namespace HC {
    using Block = uint16_t;
    class IResource {
    public:
        explicit IResource(const std::string& filepath) : filepath(filepath) { }
        virtual ~IResource() = default;
        virtual bool Load() = 0;
        virtual bool Save() = 0;

        std::string GetFilePath() const {
            return filepath;
        }

    protected:
        std::string filepath;
    };

    template<typename T>
    class Resource : public IResource {
    public:
        explicit Resource(const std::string& filepath) : IResource(filepath) { }
        virtual bool Load() override = 0;
        virtual bool Save() override = 0;
    };

    class FileResource : public Resource<FileResource> {
    public:
        FileResource(const std::string& filepath) : Resource(filepath), reader(filepath), writer(filepath) {

        }


        bool Load() override {
            // Do something
            reader.OpenFile();
            bytes.resize(reader.GetFileContentSize());
            reader.Read(bytes.data(), reader.GetFileContentSize());
            reader.CloseFile();
            return true;
        }

        bool Save() override {
            writer.OpenFile();
            writer.Write(bytes.data(), bytes.size());
            writer.CloseFile();
            return true;
        }
    public:
        std::vector<uint8_t> bytes;
    private:
        FileWriter writer;
        FileReader reader;


    };

    class ChunkResource : public FileResource {
    public:
        ChunkResource(const std::string& filepath) : FileResource(filepath) { }
        bool Load() override{
            bool bSuccess = FileResource::Load();
            if(bSuccess) {
                for(size_t i = 0; i < bytes.size(); i+=4) {
                    uint16_t position;
                    Block block;
                    memcpy(&position, bytes.data() + i, sizeof(uint16_t));
                    memcpy(&block, bytes.data() + i+2, sizeof(Block));
                    blocks[position] = block;
                }
            }

            return bSuccess;
        }

        bool Save() override {
            bytes.clear();
            for(auto& [position, block] : blocks) {
                bytes.push_back((uint8_t)(position & 0xFF));
                bytes.push_back((uint8_t)(position >> 8));
                bytes.push_back((uint8_t)(block & 0xFF));
                bytes.push_back((uint8_t)(block >> 8));
            }
            return FileResource::Save();
        }
        std::map<uint16_t, Block> blocks;


    };

    class ResourceManager : public Singleton<ResourceManager> {
    public:
        template<typename T>
        std::shared_ptr<T> Load(const std::string& filepath) {
            std::shared_ptr<T> resource;
            auto it = resources.find(filepath);
            if (it != resources.end()) {
                resource = std::dynamic_pointer_cast<T>(it->second);
            }

            if (!resource) {
                resource = std::make_shared<T>(filepath);
                // if the loading is not successful, we reset the newly created ptr
                if (!resource->Load()) {
                    resource.reset();
                    return nullptr;
                }
                resources[filepath] = std::static_pointer_cast<IResource>(resource);
            }
            return resource;
        }

        bool Unload(const std::string& filepath) {
            auto it = resources.find(filepath);
            if (it != resources.end()) {
                resources.erase(it);
                return true;
            }
            return false;
        }

    private:
        std::map<std::string, std::shared_ptr<IResource>> resources;
    };

}