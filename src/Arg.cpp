
/****************************************************************************** 
 * 
 *  file:  Arg.cpp
 * 
 *  Copyright (c) 2003, Michael E. Smoot .
 *  All rights reverved.
 * 
 *  See the file COPYING in the top directory of this distribution for
 *  more information.
 *  
 *  THE SOFTWARE IS PROVIDED _AS IS_, WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 *  DEALINGS IN THE SOFTWARE.  
 *  
 *****************************************************************************/ 


#include <tclap/Arg.h>

namespace TCLAP {

// defaults
bool Arg::_ignoreRest = false;
char Arg::_delimiter = ' ';
const char Arg::blankChar = '*';
const char Arg::flagStartChar = '-';
const string Arg::flagStartString = "-";
const string Arg::nameStartString = "--";

Arg::Arg( const string& flag, 
		 const string& name, 
		 const string& desc, 
		 bool req,
		 bool valreq,
		 Visitor* v)
: 
  _flag(flag),
  _name(name),
  _description(desc),
  _required(req),
  _requireLabel("required"),
  _valueRequired(valreq),
  _alreadySet(false),
  _visitor( v ),
  _ignoreable(true),
  _xorSet(false)
{ 
	if ( _flag.length() > 1 ) 
		throw(ArgException("Argument flag can only be one character long",
							toString() ) );
};

Arg::Arg()
: 
  _flag(""),
  _name(""),
  _description(""),
  _required(false),
  _requireLabel("required"),
  _valueRequired(false),
  _alreadySet(false),
  _visitor( NULL ),
  _ignoreable(false),
  _xorSet(false)
{ };

Arg::Arg(const Arg& a)
: 
  _flag(a._flag),
  _name(a._name),
  _description(a._description),
  _required(a._required),
  _requireLabel(a._requireLabel),
  _valueRequired(a._valueRequired),
  _alreadySet(a._alreadySet),
  _visitor( a._visitor ),
  _ignoreable(a._ignoreable),
  _xorSet(a._xorSet)
{ };

Arg::~Arg() { };

Arg& Arg::operator=(const Arg& a)
{
	if ( this != &a )
	{
		_flag = a._flag;
		_name = a._name;
		_description = a._description;
		_required = a._required;
		_requireLabel = a._requireLabel;
		_valueRequired = a._valueRequired;
		_alreadySet = a._alreadySet;
  		_visitor = a._visitor; 
  		_ignoreable = a._ignoreable;
  		_xorSet = a._xorSet;
	} 
	return *this;
};

string Arg::shortID( const string& valueId ) const
{
	string id = "";

	id = Arg::flagStartString + _flag;

	string delim = " "; 
	delim[0] = Arg::_delimiter; // ugly!!!
	
	if ( _valueRequired )
		id += delim + "<" + valueId  + ">";

	if ( !_required )
		id = "[" + id + "]";

	return id;
}

string Arg::longID( const string& valueId ) const
{
	string id = "";

	id = Arg::flagStartString + _flag;

	if ( _valueRequired )
		id += " <" + valueId + ">";


	id += ",  " + Arg::nameStartString + _name;

	if ( _valueRequired )
		id += " <" + valueId + ">";
			
	return id;

}

bool Arg::operator==(const Arg& a)
{
	if ( _flag == a._flag || 
		 _name == a._name || 
		 _description == a._description )
		return true;
	else
		return false;
}

// should be overridden
bool Arg::processArg(int* i, vector<string>& args)
{
	cerr << "WARNING:   Ignoring unknown argument: " << args[*i] << endl;	
	return false;
}

const string& Arg::getName() const { return _name; } ;

string Arg::getDescription() const 
{
	string desc = "";
	if ( _required )
		desc = "(" + _requireLabel + ")  ";

	if ( _valueRequired )
		desc += "(value required)  ";

	desc += _description;
	return desc; 
};

const string& Arg::getFlag() const { return _flag; };
bool Arg::isRequired() const { return _required; }
bool Arg::isValueRequired() const { return _valueRequired; }

bool Arg::isSet() const 
{ 
	if ( _alreadySet && !_xorSet )
		return true;
	else
		return false;
}

bool Arg::isIgnoreable() const { return _ignoreable; }

void Arg::setRequireLabel( const string& s) 
{ 
	_requireLabel = s;
}

bool Arg::argMatches( const string& argFlag ) const
{
	if ( argFlag == Arg::flagStartString + _flag ||
		 argFlag == Arg::nameStartString + _name )
		return true;
	else
		return false;
}

string Arg::toString() const
{
	string s = Arg::flagStartString + _flag + " (" + Arg::nameStartString + _name + ")";
	return s;
}

void Arg::_checkWithVisitor() const
{
	if ( _visitor != NULL )
		_visitor->visit();
}

/**
 * Implementation of trimFlag.
 */
void Arg::trimFlag(string& flag, string& value) const
{
	int stop = 0;
	for ( int i = 0; (unsigned int)i < flag.length(); i++ )
		if ( flag[i] == Arg::_delimiter )
		{
			stop = i;
			break;
		}

	if ( stop > 1 )
	{
		value = flag.substr(stop+1);
		flag = flag.substr(0,stop);
	}

}

/**
 * Implementation of _hasBlanks.
 */
bool Arg::_hasBlanks( const string& s ) const
{
	for ( int i = 1; (unsigned int)i < s.length(); i++ )
		if ( s[i] == Arg::blankChar )
			return true;

	return false;
}

void Arg::forceRequired()
{
	_required = true;
}

void Arg::xorSet()
{
	_alreadySet = true;
	_xorSet = true;
}

}
