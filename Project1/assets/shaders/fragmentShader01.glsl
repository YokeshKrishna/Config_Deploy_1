#version 420

in vec4 color;
in vec4 vertWorld;
in vec4 Normal;


uniform vec4 objectColor;
uniform vec4 specularColor;

uniform vec4 defaultColor;
uniform bool isLightOff;


uniform vec4 eyeLocation;

out vec4 pixelcolor;

struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 1;
uniform sLight Lights[NUMBEROFLIGHTS];

vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );


//vec4 light_on_object(vec3 lightPos, vec3 Normal, vec3 object_color, vec4 vWorld);

void main()
{
    //gl_FragColor = vec4(color, 1.0);
   //gl_FragColor = vec4(newColour, 1.0);
   
//	float dist = distance( lightPosition, vertWorld.xyz );
//	float linearAtten = 0.1f;    
//	float atten = 1.0f / ( linearAtten * dist );
//	pixelcolor = newColour;  
//	pixelcolor *= atten;
//
	if (isLightOff)
	{
		pixelcolor.rgb = defaultColor.rgb;
		pixelcolor.a = 1.0f;
	}

	vec4 outcolor = calcualteLightContrib(objectColor.rgb, Normal.xyz, vertWorld.xyz, specularColor);

	pixelcolor = outcolor;
	
	
	//pixelcolor = light_on_object(lightPosition, Normal.xyz, newColour.rgb, vertWorld);
	
	
}

//vec4 light_on_object(vec3 lightPos, vec3 Normal, vec3 object_color, vec4 vWorld)
//{
//	vec3 _normal = normalize(Normal);
//
//	vec4 final_out_color = vec4(0.0f,0.0f,0.0f,1.0f);
//
//	vec3 light_to_vertex = lightPos.xyz - vWorld.xyz;
//
//	float _length = length(light_to_vertex);
//
//	vec3 light_vector = normalize(light_to_vertex);
//
//	float dotproduct = dot(light_vector, vWorld.xyz);
//
//	dotproduct = max(0.0f, dotproduct);
//
//	vec3 lightDiffuse = dotproduct * objectColor.xyz;
//
//	vec3 lightSpecular = vec3(0.0f);
//
//	vec3 reflectionVector = reflect( -light_vector, normalize(_normal.xyz));
//
//	vec3 eyeVector = normalize(eyeLocation.xyz - vWorld.xyz);
//
//	float SpecularPower = 1000.0f;
//
//	vec3 specularColor = vec3(1.0f);
//
//	lightSpecular = pow( max( 0.0f, dot(eyeVector, reflectionVector)), SpecularPower) * vec3(1.0f);
//
//	float atten = 1.0f / _length;
//
//	lightDiffuse *= atten;
//	lightSpecular *= atten;
//
//	final_out_color.rgb += (object_color.rgb * lightDiffuse) + ( specularColor.rgb * lightSpecular);
//	final_out_color.a = 1.0f;
//
//	return final_out_color;
//}
//
vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( Lights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(Lights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = Lights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -Lights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
		
			lightContrib *= dotProduct;		
			
			finalObjectColour.rgb += (vertexMaterialColour.rgb * Lights[index].diffuse.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)


			return finalObjectColour;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = Lights[index].position.xyz - vertexWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		// -1 to 1
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		// If it's negative, will clamp to 0 --- range from 0 to 1
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * Lights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = vertexSpecular.w; 
		
//		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
//			                   * vertexSpecular.rgb;	//* Lights[lightIndex].Specular.rgb
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * Lights[index].specular.rgb;
							   
		// Attenuation
		float attenuation = 1.0f / 
				( Lights[index].atten.x + 										
				  Lights[index].atten.y * distanceToLight +						
				  Lights[index].atten.z * distanceToLight*distanceToLight );  	
				  
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
		

			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - Lights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, Lights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(Lights[index].param1.z));
			float innerConeAngleCos = cos(radians(Lights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
		
		//lightSpecularContrib.rgb *= 0.01f;
		//lightSpecularContrib.rgb += vec3(0.0f,0.0f,0.0f);

		//lightDiffuseContrib.rgb *= 0.01f;
		//lightDiffuseContrib.rgb += vec3(0.0f, 1.0f, 0.0f);
		
					
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
								  + (vertexSpecular.rgb  * lightSpecularContrib.rgb );

	}//for(intindex=0...
	
	finalObjectColour.a = 1.0f;
	
	return finalObjectColour;
}