
README.TXT for the Views Slideshow Xtra (VSX) Module 

The purpose of this module is to create an easy way to place HTML elements
(text, links, icons, etc) on top of a Views Slideshow.  The HTML elements are placed in 
overlays. There are two types of overlays: one that is always present, and another that
is invoked as a lightbox.  The elements are created by entering text and other formatting
information in specially named fields in the nodes that are used for the Views Slideshow.

There is an example implementation of this module at http://drupalanswer.com/vsx.  This
page has both SingleFrame and ThumbnailHover examples, and both examples are on one page,
demonstrating that multiple slideshows may be on a single page.


The supported overlay field types are:

VSX Type: Text
CCK Field Name: field_vsx_text
CCK Field Type: Text
Format: {top:999, left:999}Text Here

VSX Type: Link
CCK Field Name: field_vsx_link
CCK Field Type: Text
Format: {top:999, left:999, link:"http://example.com"}Link Text Here

Type: Lightbox Link
CCK Field Name: field_vsx_lightbox_link
CCK Field Type: Text
Format: {top:999, left:999, link:"http://example.com"}Lightbox Link Text Here


The supported Lightbox field types are:

Type: Lightbox Title
CCK Field Name: field_vsx_lightbox_title
CCK Field Type: Text
Format: Lightbox Title

Type: Lightbox Text
CCK Field Name: field_vsx_lightbox_text
CCK Field Type: Text
Format: Lightbox Text

Type: Lightbox Video
CCK Field Name: field_vsx_lightbox_video
CCK Field Type: Embedded Video
Format: Any valid embedded video string, e.g. a link to a YouTube video, etc.


Some Notes:

1. There is an 850 millisecond delay before the overlay elements are displayed.  
This was added to avoid issues with font pixelation in IE that may occur
when text is located in an area where a jQuery Cycle transition takes place.
Eventually the length of this delay will be configurable by slide show and element type.

2. To fine tune your slideshow, you will need to override some of the CSS rules found in 
views-slideshow-xtra.css, located in the module directory.  Don't change views-slideshow-xtra.css,
but rather override its CSS rules as required in your theme's CSS files.

3. Layout of the lightbox fields is controlled by rules in views-slideshow-xtra.css.  The default 
layout has the title across the top and lightbox text and video floated left below the title. You can 
change this by overriding the floats, heights, padding, etc. found in views-slideshow-xtra.css.

4. Whenever the mouse is moved over the slideshow, the show is paused for two seconds.  This is 
done to prevent the situation where a user moves the mouse pointer towards a link and the slide
transitions before the user is able to click the link.  As long as the mouse is moving, the 
slides will not transition for two seconds.
 
5. To put a mask over the slideshow, apply a background image to the div with class 
".views-slideshow-xtra-overlay".

