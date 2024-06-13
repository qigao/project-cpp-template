#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <vector>

constexpr int totalFileSize = 1000000;
constexpr int chunkSize = 100000;
std::vector<char> fileData(totalFileSize);
std::mutex fileMutex;

int downloadChunk(int startIndex)
{
    // Simulate downloading a chunk of data from the internet
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::lock_guard<std::mutex> lock(fileMutex);
    for (int i = 0; i < chunkSize; ++i)
    {
        fileData[startIndex + i] = 'X'; // Mark downloaded part
    }

    std::cout << "\nDownloaded block: " << startIndex << "-"
              << startIndex + chunkSize << std::endl;

    return (startIndex + chunkSize);
}

int downloadAndWait()
{
    // Imagine a big file waiting to be downloaded.
    std::cout << "Downloading file (downloadAndWait)...\n";

    std::vector<std::future<int>> downloadTasks;

    // Divide the file into chunks and start downloading concurrently
    for (int i = 0; i < totalFileSize; i += chunkSize)
    {
        downloadTasks.push_back(
            std::async(std::launch::async, downloadChunk, i));
    }

    // Wait for all download tasks to complete
    for (auto& task : downloadTasks)
    {
        task.wait();
    }

    // Imagine the downloaded file is ready to be used.
    std::cout << "File downloaded.\n";

    // Check if the entire file is downloaded
    bool isFileComplete = true;
    for (char c : fileData)
    {
        if (c != 'X')
        {
            isFileComplete = false;
            break;
        }
    }

    if (isFileComplete)
    {
        std::cout << "Downloaded file is complete.\n";
    }
    else
    {
        std::cout << "Downloaded file is incomplete.\n";
    }

    return 0;
}

int downloadNonBlocking()
{
    // Imagine a big file waiting to be downloaded.
    std::cout << "Downloading file (downloadNonBlocking)...\n";

    using DownloadTaskList = std::vector<std::future<int>>;
    DownloadTaskList downloadTasks;

    // Divide the file into chunks and start downloading concurrently
    for (int i = 0; i < totalFileSize; i += chunkSize)
    {
        downloadTasks.push_back(
            std::async(std::launch::async, downloadChunk, i));
    }

    while (!downloadTasks.empty())
    {
        std::cout << "loop begin[";
        DownloadTaskList::iterator task = downloadTasks.begin();
        while (task != downloadTasks.end())
        {
            std::future_status fStat =
                task->wait_for(std::chrono::milliseconds(1));
            if (fStat == std::future_status::timeout)
            {
                std::cout << ".";
                ++task;
            }
            else
            {
                try
                {
                    std::cout
                        << "Downloaded file block end pos: " << task->get()
                        << '\n';
                }
                catch (std::exception const& e)
                {
                    std::cout << "Exception " << e.what() << '\n';
                }
                task = downloadTasks.erase(task);
            }
        }
        std::cout << "] loop end" << std::endl;
    }

    // Imagine the downloaded file is ready to be used.
    std::cout << "File downloaded.\n";

    // Check if the entire file is downloaded
    bool isFileComplete = true;
    for (char c : fileData)
    {
        if (c != 'X')
        {
            isFileComplete = false;
            break;
        }
    }

    if (isFileComplete)
    {
        std::cout << "Downloaded file is complete.\n";
    }
    else
    {
        std::cout << "Downloaded file is incomplete.\n";
    }

    return 0;
}

int main()
{
    downloadAndWait();
    downloadNonBlocking();
}
