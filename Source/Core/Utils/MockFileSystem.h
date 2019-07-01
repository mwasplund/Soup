﻿// <copyright file="MockFileSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IFileSystem.h"

namespace Soup
{
    export enum class FileSystemRequestType
    {
        Exists,
        GetLastWriteTime,
        OpenRead,
        OpenWrite
    };

    export struct MockFileState
    {
        MockFileState(std::stringstream contents, std::time_t lastWriteTime) :
            Contents(std::make_shared<std::stringstream>(std::move(contents))),
            LastWriteTime(lastWriteTime)
        {
        }

        MockFileState(std::stringstream contents) :
            MockFileState(std::move(contents), std::time_t())
        {
        }

        MockFileState(std::time_t lastWriteTime) :
            MockFileState(std::stringstream(""), lastWriteTime)
        {
        }

        MockFileState() :
            MockFileState(std::stringstream(""))
        {
        }

        std::shared_ptr<std::stringstream> Contents;
        std::time_t LastWriteTime;
    };

    /// <summary>
    /// The mock file system
    /// TODO: Move into test project
    /// </summary>
    export class MockFileSystem : public IFileSystem
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref='MockFileSystem'/> class.
        /// </summary>
        MockFileSystem() :
            _requests(),
            _files()
        {
        }

        /// <summary>
        /// Get the load requests
        /// </summary>
        const std::vector<std::pair<std::string, FileSystemRequestType>>& GetRequests() const
        {
            return _requests;
        }

        /// <summary>
        /// Create a test file
        /// </summary>
        void CreateFile(Path path, MockFileState state)
        {
            _files.emplace(
                path,
                std::move(state));
        }

        /// <summary>
        /// Gets a value indicating whether the directory/file exists
        /// </summary>
        virtual bool Exists(const Path& path) override final
        {
            _requests.push_back(std::make_pair(path.ToString(), FileSystemRequestType::Exists));
            auto file = _files.find(path);
            return file != _files.end();
        }

        /// <summary>
        /// Get the last write time of the file/directory
        /// </summary>
        virtual std::time_t GetLastWriteTime(const Path& path) override final
        {
            _requests.push_back(std::make_pair(path.ToString(), FileSystemRequestType::GetLastWriteTime));
            auto file = _files.find(path);
            if (file != _files.end())
            {
                return file->second.LastWriteTime;
            }
            else
            {
                auto message = "Cannot find file for last write time: " + path.ToString();
                throw std::runtime_error(message);
            }
        }

        /// <summary>
        /// Open the requested file as a stream to read
        /// </summary>
        virtual std::shared_ptr<std::istream> OpenRead(const Path& path) override final
        {
            _requests.push_back(std::make_pair(path.ToString(), FileSystemRequestType::OpenRead));
            auto file = _files.find(path);
            if (file != _files.end())
            {
                return file->second.Contents;
            }
            else
            {
                auto message = "Cannot open read: " + path.ToString();
                throw std::runtime_error(message);
            }
        }

        /// <summary>
        /// Open the requested file as a stream to write
        /// </summary>
        virtual std::shared_ptr<std::ostream> OpenWrite(const Path& path) override final
        {
            _requests.push_back(std::make_pair(path.ToString(), FileSystemRequestType::OpenWrite));
            auto file = _files.find(path);
            if (file != _files.end())
            {
                return file->second.Contents;
            }
            else
            {
                // Create the file if it does not exist
                auto insert = _files.emplace(path, MockFileState());
                return insert.first->second.Contents;
            }
        }

    private:
        std::vector<std::pair<std::string, FileSystemRequestType>> _requests;
        std::map<Path, MockFileState> _files;
    };
}
