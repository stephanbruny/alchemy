#version 330
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

// Make a nice spectrum of colors based on counter and maxSize
vec4 Colorizer(float counter, float maxSize)
{
    float red = 0.0, green = 0.0, blue = 0.0;
    float normsize = counter/maxSize;
    
    red = smoothstep(0.3, 0.7, normsize);
    green = sin(3.14159*normsize);
    blue = 1.0 - smoothstep(0.0, 0.4, normsize);
    
    return vec4(0.8*red, 0.8*green, 0.8*blue, 1.0);
}

void main()
{
	vec4 color = vec4(1.0);
	float scale = 1000.0; // Makes 100x100 square grid. Change this variable to make a smaller or larger grid.
	int value = int(scale*floor(fragTexCoord.y*scale)+floor(fragTexCoord.x*scale));  // Group pixels into boxes representing integer values

    if ((value == 0) || (value == 1) || (value == 2)) finalColor = vec4(1.0);
    else
    {
        for (int i = 2; (i < max(2, sqrt(value) + 1)); i++) 
        {
            if ((value - i*floor(float(value)/float(i))) == 0)
            {
                color = Colorizer(float(i), scale);
                //break;    // Uncomment to color by the largest factor instead
            }
        }

        finalColor = color;
    }
}