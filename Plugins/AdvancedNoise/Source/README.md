# Advanced Noise Plugin
## [Mario GC](https://github.com/Ie-Karma)

## Overview

The **Advanced Noise Plugin** is a custom Unreal Engine plugin designed to generate high-quality noise textures using a variety of noise algorithms. This plugin includes support for Simplex, Perlin, and Voronoi noise types. It provides a user-friendly interface for configuring noise parameters and generating textures directly within the Unreal Engine editor.

## Features

- **Noise Algorithms**: Includes implementations for Simplex, Perlin, and Voronoi noise.
- **Intuitive UI**: A custom widget for easy configuration of noise parameters and texture generation.
- **Blueprint and C++ Integration**: Access noise functions from both Blueprints and C++ code.
- **Parameter Management**: Save and load noise parameters to and from a data table.

## Usage

### Widget

1. **Open the Advanced Noise HUD**
    - In the Unreal Engine editor, navigate to the **Window** menu and select **Advanced Noise HUD** to open the custom editor utility widget.

2. **Configure Noise Parameters**
    - **Frequency**: Set the frequency of the noise.
    - **Amplitude**: Set the amplitude of the noise.
    - **Lacunarity**: Set the lacunarity of the noise.
    - **Persistence**: Set the persistence of the noise.
    - **Texture Size (X, Y)**: Specify the width and height of the texture.
    - **Texture Name**: Enter a name for the generated texture.
    - **Export Path**: Specify the path where the texture will be saved.
    - **Randomize Seed**: Check this box to randomize the seed value.
    - **Seed**: Set the seed value for noise generation.
    - **Select Noise Type**: Choose the noise type from the dropdown menu. Available options are:
        - Simplex
        - Perlin
        - Voronoi (To be implemented)
    - **Generate Noise**: Click the **Generate Noise** button to create the noise texture based on the configured parameters. The texture will be saved to the specified export path.
    - **Save Parameters**: Click the **Save Parameters** button to save the current noise parameters to a data table for later use.
    - **Load Parameters**: Use the **Load Parameters** dropdown to select and load previously saved noise parameters.

## C++ and Blueprint Integration

### Noise Functions

The plugin provides several noise functions that can be called from both C++ and Blueprints:

#### [NoiseGenerator.h](Source/AdvancedNoise/Public/Noise/NoiseGenerator.h)

```cpp
    UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
    static float SimplexNoise(float X, float Y, float Frequency, float Amplitude, float Lacunarity, float Persistence, const int Seed = 0);

    UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
    static float PerlinNoise(const float X, const float Y, const float Frequency, const float Amplitude, const int Seed);

    UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
    static float VoronoiNoise(float X, float Y, float CellSize, float Amplitude, int Seed, int Width, int Height);
```

##### Example Usage

```cpp
    switch (NoiseType)
    {
    case EAdvancedNoiseType::Simplex:
        NoiseValue = UAdvancedNoise::SimplexNoise(
            (x / 10.0f), (y / 10.0f), MFrequency, MAmplitude, MLacunarity, MPersistence, Seed);
        break;
    case EAdvancedNoiseType::Perlin:
        NoiseValue = UAdvancedNoise::PerlinNoise(
            (x / 10.0f), (y / 10.0f),MFrequency, MAmplitude, Seed);
        break;
    case EAdvancedNoiseType::Voronoi:
        NoiseValue = UAdvancedNoise::VoronoiNoise(
            (x / 10.0f), (y / 10.0f), MCellSize, MAmplitude, Seed, Width, Height);
        break;
    }
```

These functions can be used to get the noise value at specific coordinates or to generate a noise texture programmatically.

### Texture Generation

#### [NoiseTextureGenerator.h](Source/AdvancedNoise/Public/Texture/TextureGen.h)

```cpp
    UFUNCTION(BlueprintCallable, Category = "Advanced Noise")
    static void NewTexture(const FVector2D& GridSize, const int Seed, const FString& Name, const FString& TextureDirectory, const FNoiseParameters& TextNoiseValues, const float Scatter, const EAdvancedNoiseType NoiseType);
```
This function can be called from both C++ and Blueprint to generate a noise texture with the specified parameters.