﻿# Sentiment Analyzer Windows Vision Skill sample

Provides an end-to-end sample to show how to write a Windows Vision Skill by extending the [Microsoft.AI.Skills.SkillInterfacePreview](./doc/Microsoft.AI.Skills.SkillInterfacePreview.md) base API.
This skill's implementation leverages the Windows built-in FaceDetector and Windows.AI APIs along a Machine learning model in ONNX format to identify a face in an image and infer its sentiment.
This sample also contains script to package the skill's Windows Runtime Component and its assets to a nuget package (.nupkg) a UWP application that ingests this nuget package and exercises the skill against images.

Specifically, this sample shows how to:

1. **Create and implement a skill descriptor class** inherited from ISkillDescriptor that provide information on the skill, provides a list of supported execution devices (CPU, GPU) and acts as a factory object for the skill.
2. **Create and implement a skill class** inherited from ISkill that executes the skill logic and produces output given a set of input, and acts as a factory object for the skill binding.
3. **Create and implement a skill binding class** inherited from ISkillBinding that contains input and output variables as well as expose adapter to ease usage by developers.
4. **Create a nuget package** that is dependent on the Microsoft.AI.Skills.SkillInterfacePreview nuget package that ecapsulates a Windows Runtime Component along its assets so that an app can ingest it.
5. **Ingest a custom Windows Vision Skill** from a nuget package inside a C# UWP app to execute sentiment analysis againt images by binding VideoFrame instances and displaying binding outputs via adapter properties.

## Scenario
A developer wants to expose a functionality to his/her users that infers the sentiment of a person from an image. In order to hide the complexity of the implementation and to ease the development of that functionality, the developer opts to leverage the SkillInterface APIs, extending the main classes to leverage the consistent API paradigm it introduces. This allows the developer to expose the compatible execution devices and deal internally with the complexity of the execution pipeline without imposing it to the consumer. A simplified view of a 2-step pipeline may look like this: 
 
 ![SkillLogic1](./doc/SkillLogic1.jpg)

The initialization of this pipeline is not pictured here but each step needs its own initialization routine which also adds complexity. What the consumer really cares about is feeding input, triggering the execution of the processing and retrieving the expected output: 
 
 ![SkillLogic2](./doc/SkillLogic2.jpg)

However, some consumers may also want to tweak and optimize part of that pipeline to avoid for example an image conversion step by feeding the correct image format in the first place or ensure execution of this functionality is less power hungry by selecting a specific execution device to run on. The skill interface API allows a developer to expose these parameters but still encapsulate complexity of a solution. 

## Related topics
**Reference**
- [Using the Face Detection effect](https://docs.microsoft.com/en-us/uwp/api/Windows.Media.FaceAnalysis.FaceDetector)
- [Using Windows ML](https://docs.microsoft.com/en-us/windows/ai/)
- [Acquiring the sentiment analysis ONNX model](https://github.com/onnx/models/tree/master/emotion_ferplus)

## System requirements

**Client:** Windows 10 build 17763 or greater

## Build the sample

1. If you download the samples ZIP, be sure to unzip the entire archive, not just the folder with the sample you want to build.
2. To build the C# version of the skill, you can run the included powershell script under *build\BuildSentimentAnalyzer_CS.ps1*
3. Once the sample is built, to generate a nuget package from it, you can run the included powershell script under *build\PackageSentimentAnalyzer_CS.ps1*

## Run the sample

To run the test app and visualize the sample code for the skill:
- **For the c# sample**, open the solution file located at *cs\FaceSentimentAnalysis_CS.sln*

in order for local nuget packages to be available to your app project, you need to add a local nuget repository pointing to where you built the skill nuget package and where the base API package resides (i.e. in ./*build*/). Follow the below steps to achieve this:
1. In Visual Studio, go to *Tools* \> *Nuget Package Manager* \> *Package Manager Settings*
![LocalNugetHowTo1](./doc/localNugetHowTo1.jpg)

2. Click on the *+* sign to add a new nuget source and make sure the value of the source is the path to the folder where the *.nupkg* files are (i.e. in *./build/*)
![LocalNugetHowTo2](./doc/localNugetHowTo2.jpg)

From your test app project, make sure you install the skill nuget package by right-clicking on your project \> *Manage Nuget Packages*, then make sure the *Package Source* points to your custom nuget source, then click *Install*
3. ![LocalNugetHowTo3](./doc/localNugetHowTo3.jpg)


# Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.microsoft.com.

When you submit a pull request, a CLA-bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., label, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.