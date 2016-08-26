/******************************************************************************
 *
 * Copyright (C) 2006-2016 by
 * The Salk Institute for Biological Studies and
 * Pittsburgh Supercomputing Center, Carnegie Mellon University
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
******************************************************************************/

#include <string.h>
#include <string>
#include <cstring>
#include <cctype>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeinfo>

using namespace std;

#define JSON_VAL_UNDEF -1   ///< This item holds a type that hasn't been defined yet
#define JSON_VAL_NULL 0     ///< This item holds a JSON "null"
#define JSON_VAL_TRUE 1     ///< This item holds a JSON "true"
#define JSON_VAL_FALSE 2    ///< This item holds a JSON "false"
#define JSON_VAL_NUMBER 3   ///< This item holds a JSON number (may be integer or floating point)
#define JSON_VAL_STRING 4   ///< This item holds a JSON string
#define JSON_VAL_ARRAY 5    ///< This item holds a JSON array (similar to a Python list)
#define JSON_VAL_OBJECT 6   ///< This item holds a JSON object (similar to a Python dictionary)
#define JSON_VAL_KEYVAL 7   ///< This item holds a JSON key/value pair (similar to a key/value pair in a dictionary)

// Start of header
// namespace JSON_API {

  /** Base class for all JSON item types */
  class item {
   public:
    int type  = JSON_VAL_UNDEF;
    virtual void dump(int n) = 0;
    virtual void dump_nobar(int n) = 0;
    void indent ( int n ) {
      for (int i=0; i<n; i++) {
        cout << "   ";
      }
    }
  };

  /** Class for a JSON "null" item type */
  class item_null : public item {
   public:
    item_null() {
      this->type = JSON_VAL_NULL;\
    }
    void dump(int n) {
      indent ( n );
      cout << "|-Null: " << endl;
    }
    void dump_nobar(int n) {
      indent ( n );
      cout << "Null: " << endl;
    }
  };

  /** Class for a JSON "true" item type */
  class item_true : public item {
   public:
    item_true() {
      this->type = JSON_VAL_TRUE;
    }
    void dump(int n) {
      indent ( n );
      cout << "|-True: " << endl;
    }
    void dump_nobar(int n) {
      indent ( n );
      cout << "True: " << endl;
    }
  };

  /** Class for a JSON "false" item type */
  class item_false : public item {
   public:
    item_false() {
      this->type = JSON_VAL_FALSE;
    }
    void dump(int n) {
      indent ( n );
      cout << "|-False: " << endl;
    }
    void dump_nobar(int n) {
      indent ( n );
      cout << "False: " << endl;
    }
  };

  /** Class for a JSON "number" item type (may be integer or floating point) */
  class item_number : public item {
   public:
    int as_integer;
    double as_double;
    item_number() {
      this->type = JSON_VAL_NUMBER;
      this->as_integer = 0;
      this->as_double = 0;
    }
    item_number ( int i ) {
      this->type = JSON_VAL_NUMBER;
      this->as_integer = i;
      this->as_double = i;
    }
    item_number ( double d ) {
      this->type = JSON_VAL_NUMBER;
      this->as_integer = d;
      this->as_double = d;
    }
    void dump(int n) {
      indent ( n );
      cout << "|-Number: " << to_string(as_integer) << " or " << to_string(as_double) << endl;
    }
    void dump_nobar(int n) {
      indent ( n );
      cout << "Number: " << to_string(as_integer) << " or " << to_string(as_double) << endl;
    }
  };

  /** Class for a JSON string item type */
  class item_string : public item {
   public:
    string s;
    item_string() {
      this->type = JSON_VAL_STRING;
      this->s = "";
    }
    item_string( string s ) {
      this->type = JSON_VAL_STRING;
      this->s = s;
    }
    void dump(int n) {
      indent ( n );
      cout << "|-String: \"" << s << "\"" << endl;
    }
    void dump_nobar(int n) {
      indent ( n );
      cout << "String: \"" << s << "\"" << endl;
    }
  };

  /** Class for a JSON array item type */
  class item_array : public item {
   public:
    vector<item*> *items;
    item_array() {
      this->type = JSON_VAL_ARRAY;
      this->items = new vector<item *>;
    }
    void dump(int n) {
      int len = items->size();
      indent ( n );
      cout << "|-Array contains " << to_string(len) << " items." << endl;
      n += 1;
      for (int i=0; i<len; i++) {
        //indent ( n );
        // cout << "|-Item " + to_string(i) << endl;
        item *it = items->at(i);
        it->dump(n);
      }
      n += -1;
    }
    void dump_nobar(int n) {
      dump(n);
    }
  };


  /** Class for a JSON object item type */
  class item_object : public item {
   public:
    unordered_map<string, item*> items;
    item_object() {
      this->type = JSON_VAL_OBJECT;
    }
    void dump(int n) {
      int len = items.size();
      indent ( n );
      cout << "|-Object contains " << to_string(len) << " items." << endl;
      n += 1;
      for ( auto it = items.begin(); it != items.end(); ++it ) {
        //indent ( n+1 );
        //cout << "Item[" << it->first << "] = ..." << endl;
        indent ( n );
        cout << "|-Object[" << it->first << "] = ";
        it->second->dump_nobar(0);
      }
      n += -1;
    }
    void dump_nobar(int n) {
      dump(n);
    }
  };

  /** Class for a JSON key/value pair item type */
  class item_keyval : public item {
   public:
    string key;
    item *val;
    item_keyval() {
      this->type = JSON_VAL_KEYVAL;
    }
    void dump(int n) {
      indent ( n );
      cout << "|-KeyValue key " << key << ":" << endl;
    }
    void dump_nobar(int n) {
      dump(n);
    }
  };

//}
// End of header



/** This class is used during the parsing to represent a token from the original text.
*   The class contains several integer pointers to keep track of the original text.
*/
class json_element {
 public:
  int type  = JSON_VAL_UNDEF;
  int start = 0;
  int end   = 0;
  int depth = 0;
  json_element(int what, int start, int end, int depth) {
    this->type = what;
    this->start = start;
    this->end = end;
    this->depth = depth;
  }
  void update_element(int what, int start, int end, int depth) {
    this->type = what;
    this->start = start;
    this->end = end;
    this->depth = depth;
  }
  string get_name () {
    if (type == JSON_VAL_UNDEF) return ( "Undefined" );
    if (type == JSON_VAL_NULL) return ( "NULL" );
    if (type == JSON_VAL_TRUE) return ( "True" );
    if (type == JSON_VAL_FALSE) return ( "False" );
    if (type == JSON_VAL_NUMBER) return ( "Number" );
    if (type == JSON_VAL_STRING) return ( "String" );
    if (type == JSON_VAL_ARRAY) return ( "Array" );
    if (type == JSON_VAL_OBJECT) return ( "Object" );
    if (type == JSON_VAL_KEYVAL) return ( "Key:Val" );
    return ( "Unknown" );
  }
};


class json_object;
class json_array;

class json_parser {
 public:

  const char *leading_number_chars = "-0123456789";
  const char *null_template = "null";
  const char *true_template = "true";
  const char *false_template = "false";

  char *text = NULL;
  vector<json_element *> elements;

  json_parser ( char *text ) {
    this->text = text;
    elements = vector<json_element *>();
  }

  void parse ( json_array *parent ) {
	  parse_element ( parent, 0, 0 );
  }

  json_element *pre_store_skipped ( int what, int start, int end, int depth ) {
    json_element *je = new json_element ( what, start, end, depth );
    elements.push_back(je);
    // System.out.println ( "Pre-Skipped " + what + " at depth " + depth + " from " + start + " to " + end );
    return je;
  }


  void post_store_skipped ( int what, int start, int end, int depth ) {
    json_element *je = new json_element ( what, start, end, depth );
    elements.push_back(je);
    // System.out.println ( "Post-Skipped " + what + " at depth " + depth + " from " + start + " to " + end );
  }

  void post_store_skipped ( int what, int start, int end, int depth, json_element *je ) {
    je->update_element ( what, start, end, depth );
    // System.out.println ( "Post-Skipped " + what + " at depth " + depth + " from " + start + " to " + end );
  }

  void dump ( int max_len ) {
    json_element *j;
    cout << "Elements contains " << elements.size() << " items." << endl;
    for (int i=0; i<elements.size(); i++) {
      j = elements.at(i);
      for (int d=0; d<j->depth; d++) {
        cout << "    ";
      }
      string local_text;
      local_text.assign (text);
      string display;
      if ( (j->end - j->start) <= max_len ) {
        display = local_text.substr(j->start,j->end-j->start);
      } else {
        display = local_text.substr(j->start,max_len-4);
      }
      cout << "|-" << j->get_name() << " at depth " << j->depth << " from " << j->start << " to " << (j->end-1) << " = " << display << endl;
    }
  }

  item *build_next_item ( int *index ) {
    json_element *j;
    j = elements.at(*index);
    // cout << "Index " << std::to_string(*index) << ": Elements contains " << elements.size() << " items." << endl;
    //cout << " *** ";
    // cout << " Level: " << std::to_string(this_depth) << ", Next Element Type = " + std::to_string(j->type) << endl;
    for (int d=0; d<j->depth; d++) { cout << "    "; }
    string local_text;
    local_text.assign (text);
    string display;
    display = local_text.substr(j->start,j->end-j->start);
    cout << "|-" << j->get_name() << " at depth " << j->depth << " from " << j->start << " to " << (j->end-1) << " = " << display << endl;
    if      ( j->type == JSON_VAL_NULL ) {
      return ( new item_null() );
    } else if ( j->type == JSON_VAL_TRUE ) {
      return ( new item_true() );
    } else if ( j->type == JSON_VAL_FALSE ) {
      return ( new item_false() );
    } else if ( j->type == JSON_VAL_NUMBER ) {
      double v = strtod ( display.c_str(), NULL );
      return ( new item_number( v ) );
    } else if ( j->type == JSON_VAL_STRING ) {
      int l = display.length();
      string unquoted = display.substr(1,l-2);
      return ( new item_string( unquoted ) );
    } else if ( j->type == JSON_VAL_ARRAY ) {
      return ( new item_array() );
    } else if ( j->type == JSON_VAL_OBJECT ) {
      return ( new item_object() );
    } else if ( j->type == JSON_VAL_KEYVAL ) {
      return ( new item_keyval() );
    }
    return ( NULL );
  }

  /** Build an item list as represented by a C++ vector template */
  item_array *build_item_list ( int *index ) {
    item_array *item_list = new item_array();

    json_element *j;
    // cout << "Index " << std::to_string(*index) << ": Elements contains " << elements.size() << " items." << endl;
    int this_depth = elements.at(*index)->depth;
    while ( ( *index < elements.size() ) && ( (j = elements.at(*index))->depth == this_depth ) ) {

      if ( j->type == JSON_VAL_ARRAY ) {
        for (int d=0; d<j->depth; d++) { cout << "    "; }
        cout << "|-Array ..." << endl;
        (*index)++;
        item_list->items->push_back ( build_item_list(index) );
        // (*index)++;
      } else if ( j->type == JSON_VAL_OBJECT ) {
        for (int d=0; d<j->depth; d++) { cout << "    "; }
        cout << "|-Object ..." << endl;
        item_list->items->push_back ( build_item_pair_list(index) );
      } else {
        //cout << "Got other ..." << endl;
        item_list->items->push_back ( build_next_item(index) );
        // item *next_item = build_next_item ( index );
        (*index)++;
      }

    }
    return item_list;
  }


  /** Build a dictionary (item pair list) as represented by a C++ unordered map template */
  item_object *build_item_pair_list ( int *index ) {
    item_object *item_pair_list = new item_object();

    // cout << "In build_item_pair_list ..." << endl;
    (*index)++;
    json_element *j;
    // cout << "Index " << std::to_string(*index) << ": Elements contains " << elements.size() << " items." << endl;
    int this_depth = elements.at(*index)->depth;
    while ( ( *index < elements.size() ) && ( (j = elements.at(*index))->type == JSON_VAL_KEYVAL ) ) {
      (*index)++;
      string key = "";

      j = elements.at(*index);
      for (int d=0; d<j->depth; d++) { cout << "    "; }
      if (j->type == JSON_VAL_STRING) {
        string local_text;
        local_text.assign (text);
        string display;
        key = local_text.substr(j->start+1,j->end-(2+j->start));
        cout << "|-In object ... with key = " << key << endl;
      } else {
        cout << "|-In object ... with unexpected key type = " << j->get_name() << endl;
      }
      (*index)++;

      j = elements.at(*index);

      if ( j->type == JSON_VAL_ARRAY ) {
        for (int d=0; d<j->depth; d++) { cout << "    "; }
        cout << "|-Array ..." << endl;
        (*index)++;
        item_pair_list->items[key] = build_item_list(index);
      } else if ( j->type == JSON_VAL_OBJECT ) {
        for (int d=0; d<j->depth; d++) { cout << "    "; }
        cout << "|-Object ..." << endl;
        item_pair_list->items[key] = build_item_pair_list(index);
      } else {
        item_pair_list->items[key] = build_next_item ( index );
        (*index)++;
      }
    }
    return item_pair_list;
  }


  int skip_whitespace ( int index, int depth ) {
    int i = index;
    int max = strlen(text);
    while ( isspace(text[i]) ) {
      i++;
      if (i >= max) {
        return ( -1 );
      }
    }
    return i;
  }

  int skip_sepspace ( int index, int depth ) {
    int i = index;
    int max = strlen(text);
    while ( (text[i]==',') || isspace(text[i]) ) {
      i++;
      if (i >= max) {
        return ( -1 );
      }
    }
    return i;
  }

  int parse_element ( void *parent, int index, int depth );
  int parse_keyval ( void *parent, int index, int depth );
  int parse_object ( void *parent, int index, int depth );
  int parse_array ( void *parent, int index, int depth );
  int parse_string ( void *parent, int index, int depth );
  int parse_number ( void *parent, int index, int depth );

};


////////////////   Internal Representation of Data after Parsing   /////////////////


//--> This was an attempt to get the typeid just one time for each object:  type_info json_object_type = typeid(new json_object());


class json_object : public unordered_map<string,void *> {
 public:
  virtual void print_self() {
    cout << "json_object" << endl;
    cout << "type = " << typeid(this).name() << endl;
  }
};

int global_depth = 0;

class json_array : public vector<void *> {
 public:
  virtual void print_self() {
    global_depth += 1;
    cout << "json_array is at " << this << endl;
    cout << "json_array contains " << this->size() << " elements" << endl;
    for (int i=0; i<this->size(); i++) {
      cout << "SUB OBJECT " << i << " at depth " << global_depth << " is a " << typeid(this[i]).name() << " at " << (void *)this << endl;
      this[i].print_self();
    }
    global_depth += -1;


    cout << "type.name = " << typeid(this).name() << endl;
    bool match = (typeid(this) == typeid(new json_array()));
    cout << "match = " << match << endl;
    match = (typeid(this) == typeid(new json_object()));
    cout << "match = " << match << endl;
    //cout << "type = " << typeid(this) << endl;
  }
};


class json_primitive {
 public:
  string text;
  virtual void print_self() {
    cout << "json_primitive" << endl;
  }
};

class json_number : public json_primitive {
 public:
  double value = 0.0;
  bool as_int = false;
  json_number ( string s ) {
    this->text = s;
  }
  json_number ( int v ) {
    this->value = v;
    this->as_int = true;
  }
  json_number ( double v ) {
    this->value = v;
    this->as_int = false;
  }
  virtual void print_self() {
    cout << "json_number" << endl;
  }
};


class json_string : public json_primitive {
 public:
  json_string ( string s ) {
    this->text = s;
  }
  virtual void print_self() {
    cout << "json_string" << endl;
  }
};

class json_true : public json_primitive {
 public:
  json_true () { }
  json_true ( string s ) {
    this->text = s;
  }
  virtual void print_self() {
    cout << "json_true" << endl;
  }
};

class json_false : public json_primitive {
 public:
  json_false () { }
  json_false ( string s ) {
    this->text = s;
  }
  virtual void print_self() {
    cout << "json_false" << endl;
  }
};

class json_null : public json_primitive {
 public:
  json_null () { }
  json_null ( string s ) {
    this->text = s;
  }
  virtual void print_self() {
    cout << "json_null" << endl;
  }
};


////////////////////
// These are currently down here because I couldn't figure out how to forward reference the various json_type classes directly above
////////////////////

int json_parser::parse_element ( void *parent, int index, int depth ) {
    int start = skip_whitespace ( index, depth );
    if (start >= 0) {
      if ( text[start] == '{' ) {
        // This will add an object object to the parent
        start = parse_object ( parent, start, depth );
      } else if ( text[start] == '[' ) {
        // This will add an array object to the parent
        start = parse_array ( parent, start, depth );
      } else if ( text[start] == '\"' ) {
        // This will add a string object to the parent
        start = parse_string ( parent, start, depth );
      } else if ( strchr(leading_number_chars,text[start]) != NULL ) {
        // This will add a number object to the parent
        start = parse_number ( parent, start, depth );
      } else if ( strncmp ( null_template, &text[start], 4) == 0 ) {
        post_store_skipped ( JSON_VAL_NULL, start, start+4, depth );
        // Add a null object to the parent
        json_array *p = (json_array *)parent;
        json_null *val = new json_null;
        cout << "Created a json_null at " << (void *)val << " with parent at " << parent << endl;
        p->push_back( (void *)val );
        start += 4;
      } else if ( strncmp ( true_template, &text[start], 4) == 0 ) {
        post_store_skipped ( JSON_VAL_TRUE, start, start+4, depth );
        // Add a true object to the parent
        json_array *p = (json_array *)parent;
        json_true *val = new json_true;
        p->push_back( (void *)val );
        cout << "Created a json_true at " << (void *)val << " with parent at " << parent << endl;
        start += 4;
      } else if ( strncmp ( false_template, &text[start], 5) == 0 ) {
        post_store_skipped ( JSON_VAL_FALSE, start, start+5, depth );
        // Add a false object to the parent
        json_array *p = (json_array *)parent;
        json_false *val = new json_false;
        p->push_back( (void *)val );
        cout << "Created a json_false at " << (void *)val << " with parent at " << parent << endl;
        start += 5;
      } else {
        cout << "Unexpected char (" << text[start] << ") in " << text << endl;
      }
    }
    return start;
  }





int json_parser::parse_keyval ( void *parent, int index, int depth ) {

//////////////// NOTE: Should probably use std::pair to store these key/value pairs!!!!

    json_array *key_val = new json_array();

    json_element *je = pre_store_skipped ( JSON_VAL_KEYVAL, index, index, depth );
    index = skip_whitespace ( index, depth );
    int end = index;
    end = parse_string ( key_val, end, depth );

    end = skip_whitespace ( end, depth );
    end = end + 1;  // This is the colon separator (:)
    end = parse_element ( key_val, end, depth );
    post_store_skipped ( JSON_VAL_KEYVAL, index, end, depth, je );

    json_object *p = (json_object *)parent;
    json_string *s = (json_string *)(key_val->at(0));
    // cout << "Key = " << s->text << endl;
    p->insert ( { s->text, (void *)(key_val->at(1)) } );

    cout << "Created a json_keyval with parent at " << parent << endl;

    return (end);
  }




int json_parser::parse_object ( void *parent, int index, int depth ) {
    json_element *je = pre_store_skipped ( JSON_VAL_OBJECT, index, index, depth );
    int end = index+1;
    depth += 1;

    json_array *p = (json_array *)parent;
    json_object *o = new json_object();
    p->push_back ( (void *)o );

    cout << "Created a json_object at " << (void *)o << " with parent at " << parent << endl;

    while (text[end] != '}') {
      end = parse_keyval ( o, end, depth );
      end = skip_sepspace ( end, depth );
    }
    depth += -1;
    post_store_skipped ( JSON_VAL_OBJECT, index, end+1, depth, je );

    return (end + 1);
  }



int json_parser::parse_array ( void *parent, int index, int depth ) {

    json_element *je = pre_store_skipped ( JSON_VAL_ARRAY, index, index, depth );
    int end = index+1;
    depth += 1;

    json_array *p = (json_array *)parent;
    json_array *child = new json_array();
    p->push_back ( (void *)child );

    cout << "Created a json_array at " << (void *)child << " with parent at " << parent << endl;

    while (text[end] != ']') {
      end = parse_element ( child, end, depth );
      end = skip_sepspace ( end, depth );
    }
    depth += -1;
    post_store_skipped ( JSON_VAL_ARRAY, index, end+1, depth, je );
    return (end + 1);
  }


int json_parser::parse_string ( void *parent, int index, int depth ) {
    int end = index+1;
    while (text[end] != '"') {
      end++;
    }
    post_store_skipped ( JSON_VAL_STRING, index, end+1, depth );

    json_array *p = (json_array *)parent;

    string local_text;
    local_text.assign (text);

    string sub_string = local_text.substr(index+1,end-index);

    json_string *val = new json_string(sub_string);
    p->push_back( (void *)val );

    cout << "Created a json_string at " << (void *)val << " with parent at " << parent << endl;

    return (end + 1);
  }


int json_parser::parse_number ( void *parent, int index, int depth ) {
    int end = index;
    const char *number_chars = "0123456789.-+eE";
    while ( strchr(number_chars,text[end]) != NULL ) {
      end++;
    }
    post_store_skipped ( JSON_VAL_NUMBER, index, end, depth );

    json_array *p = (json_array *)parent;

    string local_text;
    local_text.assign (text);

    string sub_string = local_text.substr(index,end-index);

    json_number *val = new json_number(sub_string);
    p->push_back( (void *)val );

    cout << "Created a json_string at " << (void *)val << " with parent at " << parent << endl;

    return (end);
  }

/**
*   Main for testing.
*   This code constructs the same JSON structure from classes and from text.
*/
int main() {
  cout << "JSON C++ Parser" << endl;

  item_array *t0 = new item_array();
  item_array *t1 = new item_array();
  t1->items->push_back ( new item_string ( "one" ) );
  t1->items->push_back ( new item_number ( 2.2 ) );
  item_array *t2 = new item_array();
  t2->items->push_back ( new item_string ( "A" ) );
  t2->items->push_back ( new item_true ( ) );
  t2->items->push_back ( new item_string ( "C" ) );
  t1->items->push_back ( t2 );
  item_object *t3 = new item_object();
  t3->items["q"] = new item_number ( 7 );
  t3->items["x"] = new item_string ( "XXX" );
  t3->items["y"] = new item_string ( "YYY" );
  t3->items["z"] = new item_string ( "ZZZ" );
  t1->items->push_back ( t3 );
  t1->items->push_back ( new item_string ( "three" ) );
  t0->items->push_back ( t1 );

  cout << "====== Before Parsing ======" << endl;

  // char *text = "{\"A\":true,\"mc\":[{\"a\":0.01},1e-5,2,true,[9,[0,3],\"a\",345],false,null,5,[1,2,3],\"xyz\"],\"x\":\"y\"}";
  // char *text = "[ 9, [0,3], [1,4], { \"a\":5, \"b\":7, \"N\":null, \"TF\":{\"F\":false,\"T\":true}, \"A\":[3] }, \"END\" ]";
  char *text = "[ \"one\", 2.2, [\"A\", true, \"C\"], {\"q\":7, \"x\":\"XXX\", \"y\":\"YYY\", \"z\":\"ZZZ\"}, \"three\"]";

  json_array top;
  json_parser p = json_parser(text);
  p.parse ( &top );

  cout << "====== After Parsing ======" << endl;

  p.dump(90);

  cout << "====== While Building C++ Structures ======" << endl;

  int index = 0;
  item_array *items = p.build_item_list(&index);

  cout << "====== After Building C++ Structures ======" << endl;

  items->dump(0);

  cout << "====== Dump of equivalent Test ======" << endl;

  t0->dump(0);

  return ( 0 );
  top.print_self();

  json_element *je = new json_element(0,0,0,0);
  cout << "Hello!! " << JSON_VAL_ARRAY << endl;


  return ( 0 );
}

