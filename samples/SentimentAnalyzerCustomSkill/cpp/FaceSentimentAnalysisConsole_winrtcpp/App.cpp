// Copyright (c) Microsoft Corporation. All rights reserved.
#include "App.h"

// Function to handle the frame when it arrives from FrameReader
winrt::Windows::Foundation::IAsyncAction App::FrameArrivedHandler(MediaFrameReader FrameReader, MediaFrameArrivedEventArgs)
{
    //Lock context so we multiple overlapping events from FrameReader do not race for the resources.
    m_lock.lock();
    MediaFrameReference mediaFrame(nullptr);

    // Try to get the actual Video Frame from the FrameReader
    mediaFrame = FrameReader.TryAcquireLatestFrame();
    if (mediaFrame != nullptr)
    {
        auto vmFrame = mediaFrame.VideoMediaFrame();
        if (vmFrame != nullptr)
        {
            auto videoFrame = vmFrame.GetVideoFrame();

            // Set the video frame on the skill binding.
            m_FaceSentimentSkillBinding.SetInputImageAsync(videoFrame).get();

            //Evaluate sentiments in video frame using the skill
            m_Skill.EvaluateAsync(m_FaceSentimentSkillBinding).get();
            if (m_FaceSentimentSkillBinding.IsFaceFound())
            {
                auto sentiment = m_FaceSentimentSkillBinding.PredominantSentiment();
                std::string sentiments[] = {
                    "neutral ",
                    "happiness ",
                    "surprise ",
                    "sadness ",
                    "anger ",
                    "disgust",
                    "fear",
                    "contempt"
                };
                std::string outText = "\tYour sentiment looks like:  " + sentiments[(int)sentiment] + "\t\t\t\t\r";
                std::cout << outText;
            }
            else
            {
                std::cout << "\tNo face detected\t\t\t\t\t\t\r";// << std::endl;
            }
            videoFrame.Close();
        }
        mediaFrame.Close();
    }
    
    m_lock.unlock();
    co_return;
}

void App::InitCameraAndFrameSource()
{
    //Initialize media capture with default settings
    m_mediaCapture = winrt::Windows::Media::Capture::MediaCapture();
    m_mediaCapture.InitializeAsync().get();

    //Get a list of available Frame source and iterate through them to find a Video preview/record source with Color images ( and not IR/depth etc)
    auto fsIter = m_mediaCapture.FrameSources().First();
    do
    {
        auto mediaStreamType = fsIter.Current().Value().Info().MediaStreamType();
        auto sourceKind = fsIter.Current().Value().Info().SourceKind();
        std::cout << "MediaStreamType:" << (int)mediaStreamType << " MediaFrameSourceKind:" << (int)sourceKind << std::endl;
        if (
            ((mediaStreamType == MediaStreamType::VideoPreview)
                || (mediaStreamType == MediaStreamType::VideoRecord))
            && (sourceKind == MediaFrameSourceKind::Color))
        {
            break;
        }
    } while (fsIter.MoveNext());
    if (!fsIter.HasCurrent())
    {
        throw new winrt::hresult_out_of_bounds();
    }

    //Create frame reader with the FrameSource that we selected in the loop above.
    m_frameReader = m_mediaCapture.CreateFrameReaderAsync(fsIter.Current().Value()).get();

    //Set up a delegate to handle the frames when they are ready
    m_frameReader.FrameArrived({ this, &App::FrameArrivedHandler });

    //Finally start the frame reader
    m_frameReader.StartAsync().get();
}

void App::DeInitCameraAndFrameSource()
{
    //Stop Frame reader and close
    m_frameReader.StopAsync().get();
    m_frameReader.Close();
    m_frameReader = nullptr;
    m_mediaCapture.Close();
    m_mediaCapture = nullptr;
}

int App::AppMain()
{
    std::cout << "WinrtCPP Non-packaged(win32) console APP: Face it!!" << std::endl;

    //Create the FaceSentimentAnalyzer skill descriptor
    auto skillDesc = FaceSentimentAnalyzerDescriptor().as<ISkillDescriptor>();

    //Create instance of the skill
    m_Skill = skillDesc.CreateSkillAsync().get().as<FaceSentimentAnalyzerSkill>();

    //Create instance of the skill binding
    m_FaceSentimentSkillBinding = m_Skill.CreateSkillBindingAsync().get().as<FaceSentimentAnalyzerBinding>();

    //Initialize media capture and frame reader
    InitCameraAndFrameSource();
    std::cout << "\t\t\t\t\t\t\t\t...press enter to Stop" << std::endl;

    //wait for enter keypress
    while (std::cin.get() != '\n');

    std::cout << std::endl << "Key pressed.. exiting";

    //de-initialize the media capture and frame reader
    DeInitCameraAndFrameSource();
}

