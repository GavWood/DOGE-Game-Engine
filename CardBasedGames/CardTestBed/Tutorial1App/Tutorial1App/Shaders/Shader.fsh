//
//  Shader.fsh
//  Tutorial1App
//
//  Created by localadmin on 25/11/2015.
//  Copyright © 2015 localadmin. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
