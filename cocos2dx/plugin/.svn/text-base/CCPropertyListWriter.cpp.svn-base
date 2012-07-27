/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org  http://cocos2d-x.org
 Copyright (c) 2010 Максим Аксенов
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CCPropertyListWriter.h"
//#include <libxml/tree.h>
//#include <libxml/xmlmemory.h>
#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>

#include "CCLibxml2.h"
#include "CCNumber.h"
#include <typeinfo>

NS_CC_BEGIN;

//#define MY_ENCODING "ISO-8859-1"
#define  MY_ENCODING NULL

// pList DTD: (array | data | date | dict | real | integer | string | true | false )
CCPropertyListWriter::CCPropertyListWriter()
{

}

CCPropertyListWriter::~CCPropertyListWriter(void)
{
    
}

void CCPropertyListWriter::openPlistTag()
{
    int rc;
    
    rc = xmlTextWriterStartElement(writer, BAD_CAST "plist");
    if (rc < 0) {
        CCLog("Warning: CCPropertyListWriter::openPlistTag StartElement Error");
        return;
    }
    
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version", BAD_CAST "1.0");
    if (rc < 0) {
        CCLog("Warning: CCPropertyListWriter::openPlistTag WriteAttribute Error");
        return;
    }    
}

void CCPropertyListWriter::closePlistTag()
{
    int rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        CCLog("Warning: CCPropertyListWriter::closePlistTag Error");
        return;
    }
}
void CCPropertyListWriter::writeValue(CCObject* aValue)
{
    int rc;
    
    if( CCNumber<int>::isSameType( aValue ) )
    {
        CCNumber<>*num = (CCNumber<>*)aValue;
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "integer", "%d", num->getValue() );
    }
    else if( CCNumber<unsigned int>::isSameType( aValue ) )
    {
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "integer", "%d", ((CCNumber<unsigned int>*)aValue)->getValue() );   
    }
    else if( CCNumber<long>::isSameType( aValue ) )
    {
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "integer", "%ld", ((CCNumber<long>*)aValue)->getValue() ); 
    }
    else if( CCNumber<unsigned long>::isSameType( aValue ))
    {
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "integer", "%ld", ((CCNumber<unsigned long>*)aValue)->getValue() ); 
    }
    
    else if( CCNumber<float>::isSameType( aValue ) )
    {
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "real", "%f", ((CCNumber<float>*)aValue)->getValue() ); 
    }
    else if ( CCNumber<double>::isSameType( aValue ) ) 
    {
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "real", "%f", ((CCNumber<double>*)aValue)->getValue() ); 
    }
    else if( CCNumber<bool>::isSameType( aValue ) ) {
        rc = xmlTextWriterWriteString(writer, BAD_CAST( ((CCNumber<bool>*)aValue)->getValue() ? "<true/>" : "<false/>") );
    }
    else if( aValue->getClassName() == "CCString" )
    {
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "string", "%s", ((CCString*)aValue)->toStdString().c_str() );                 
    }
    else {
        CCAssert(0, "not ready for this type");
    }
}
void CCPropertyListWriter::writeArray(CCArray* aArray)
{
    int rc;
    //<array>
    rc = xmlTextWriterStartElement(writer, BAD_CAST "array");
    CCObject* value;
    CCARRAY_FOREACH(aArray, value)
    {        
        //<array>
        if( value->getClassName() == "CCArray" )
        {
            writeArray((CCArray*)value);
        }
        //<dict>
        else if ( value->getClassName() == "CCMutableDictionary" )
        {
            writeDictionary((CCMutableDictionary<>*)value);
        }
        //<value type>
        else
        {
            writeValue(value);
        } 
    }
    //<array>
    rc = xmlTextWriterEndElement(writer);
    
}
void CCPropertyListWriter::writeDictionary(CCMutableDictionary<>* aDic)
{
    
    int rc;
    //<dict>
    rc = xmlTextWriterStartElement(writer, BAD_CAST "dict");

    vector<string> keys = aDic->allKeys();
    for( int i = 0; i < keys.size(); i++ )
    {
        string key = keys[i];
        //CCLog("Dic_key:%s",key.c_str());
        //<key>
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "key", "%s", key.c_str());
        CCObject* value = aDic->objectForKey(key);
        //CCLog("Dic_class:%s",value->getClassName().c_str());
        //<array>
        if( value->getClassName() == "CCArray" )
        {
            writeArray((CCArray*)value);
        }
        //<dict>
        else if ( value->getClassName() == "CCMutableDictionary" )
        {
            writeDictionary((CCMutableDictionary<>*)value);
        }
        //<value type>
        else
        {
            writeValue(value);
        }        
    }

    //</dict>
    rc = xmlTextWriterEndElement(writer);
}
                              
bool CCPropertyListWriter::writeToFile(CCDictionary<string,CCObject*>* aDic , const char *pszFile)
{
   
    int rc;
    fullPath = CCFileUtils::getWriteablePath() + pszFile;

    
    /* first, the file version */
    LIBXML_TEST_VERSION

    
    /* Create a new XmlWriter for uri, with no compression. */
    writer = xmlNewTextWriterFilename(fullPath.c_str(), 0);
    if (writer == NULL) {
        CCLog("Warning: CCPropertyListWriter::NewTextWriterFilename Error");        
        return false;
    }
    
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
        CCLog ("Warning: CCPropertyListWriter::StartDocument Error");
        return false;
    }
    
    openPlistTag();
        // write actual dictionary contents
        writeDictionary(aDic);
    
    closePlistTag();
    
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) 
    {
        CCLog("Warning: CCPropertyListWriter::EndDocument Error");
        return false;
    }
    
    xmlFreeTextWriter(writer);
    
    
    /*
    * Cleanup function for the XML library.
    */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
    xmlMemoryDump();
#endif
    
	
	return true;
}


NS_CC_END;


