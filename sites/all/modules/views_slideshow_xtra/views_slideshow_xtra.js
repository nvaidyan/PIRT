Drupal.behaviors.vsx = function(context) {
  $('.views_slideshow_singleframe_teaser_section:not(.vsx-processed),.views_slideshow_thumbnailhover_teaser_section:not(.vsx-processed)',
      context)
      .addClass('vsx-processed')
      .each(
          function(i) {

            var $slideShow = $(this);
            // Add 1 - indexes for slideshow instances begin at 1 not 0.
            var slideShowIndex = i + 1;
            $slideShow.data("slideShowIndex", slideShowIndex);

            // Get reference to Drupal js settings.
            var id = "#" + $(this).parent().attr("id");
            var viewsSlideShowType;
            var settings;
            if (id.indexOf('singleframe_main') > 0) {
              settings = Drupal.settings.viewsSlideshowSingleFrame[id];
              viewsSlideShowType = "SingleFrame";
            } else {
              settings = Drupal.settings.viewsSlideshowThumbnailHover[id];
              viewsSlideShowType = "ThumbnailHover";
            }

            // First cycle initialized to true.
            var firstCycle = true;

            // Modify the Cycle plugin before method.
            eval("settings.opts.before = "
                + _vss_graft_functions(settings.opts.before, function() {
                  currSlide = opts.currSlide;
                  nextSlide = opts.nextSlide;

                  // Determine current and next id suffix.
                  var currSuffix = $(this).parent().data("slideShowIndex")
                      + "-" + currSlide;
                  var nextSuffix = $(this).parent().data("slideShowIndex")
                      + "-" + nextSlide;

                  // Show/hide overlay elements.
                  if (firstCycle) {
                    // TODO: Make delay value configurable.
                    setTimeout(function() {
                      $(".views-slideshow-xtra-slide-" + currSuffix).show();
                    }, 850);
                    firstCycle = false;
                  } else {
                    $(".views-slideshow-xtra-slide-" + currSuffix).hide();
                    // TODO: Make delay value configurable.
                    setTimeout(function() {
                      $(".views-slideshow-xtra-slide-" + nextSuffix).show();
                    }, 850);
                  }

                }));

            // Remove pager from options, otherwise pager is duplicated.
            delete settings.opts.pager;

            // Re-inject options and inject overlays.
            $slideShow
                .cycle(settings.opts)
                .append(
                    '<div id="views-slideshow-xtra-overlay-'
                        + slideShowIndex
                        + '" class="views-slideshow-xtra-overlay"'
                        + '</div>'
                        + '<div id="views-slideshow-xtra-lightbox-bg-'
                        + slideShowIndex
                        + '" class="views-slideshow-xtra-lightbox-bg">'
                        + '</div>'
                        + '<div id="views-slideshow-xtra-lightbox-'
                        + slideShowIndex
                        + '" class="views-slideshow-xtra-lightbox">'
                        + '<div class="views-slideshow-xtra-lightbox-header">'
                        + '<div class="views-slideshow-xtra-lightbox-x"></div>'
                        + '</div>'
                        + '<div class="views-slideshow-xtra-lightbox-body clear-block"></div>'
                        + '<div class="views-slideshow-xtra-lightbox-footer"></div>'
                        + '</div>');

            // If pause on start is set, pause slideshow
            if(settings.paused) {
              viewsSlideshowSingleFramePause(settings);
            }

            // get reference to overlays (used later to inject elements)
            var $overlay = $('#views-slideshow-xtra-overlay-' + slideShowIndex);
            var $lightBox = $('#views-slideshow-xtra-lightbox-'
                + slideShowIndex);
            var $lightBoxBody = $('#views-slideshow-xtra-lightbox-'
                + slideShowIndex + ' .views-slideshow-xtra-lightbox-body');

            // Set overlay height and width the same as slideshow wrapper.
            $overlay.height($slideShow.height());
            $overlay.width($slideShow.width());

            // Process the slides.
            $(".views_slideshow_slide", $slideShow)
                .each(
                    function(i) {
                      // Slide index.
                      var slideIndex = i;

                      // Process text fields.
                      $('.views-field-field-vsx-text-value .field-content,'
                              + '.views-field-field-vsx-text-value .views-content-field-vsx-text-value',
                          this)
                          .each(

                              function(i) {
                                var content = $(this).html();
                                if (content.length == 0)
                                  return;
                                var re = /(\{[^\}]*\})/;
                                var jsonStr = re.exec(content);
                                var contentStr = content.replace(re, "");
                                var jsonObj = eval("(" + jsonStr[0] + ")");
                                var markup = '<span id="views-slideshow-xtra-text-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '" class="views-slideshow-xtra views-slideshow-xtra-text '
                                    + 'views-slideshow-xtra-slide-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '" style="top:'
                                    + jsonObj.top
                                    + 'px; left:'
                                    + jsonObj.left
                                    + 'px;">'
                                    + contentStr + '</span>';
                                $overlay.append(markup);
                                $(this).parent().remove();
                              });

                      // Process link fields.
                      $('.views-field-field-vsx-link-value .field-content,'
                              + '.views-field-field-vsx-link-value .views-content-field-vsx-link-value',
                          this)
                          .each(

                              function(i) {
                                var content = $(this).html();
                                if (content.length == 0)
                                  return;
                                var re = /(\{[^\}]*\})/;
                                var jsonStr = re.exec(content);
                                var contentStr = content.replace(re, "");
                                var jsonObj = eval("(" + jsonStr[0] + ")");
                                var markup = '<a id="views-slideshow-xtra-link-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '" class="views-slideshow-xtra views-slideshow-xtra-link '
                                    + 'views-slideshow-xtra-slide-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '" style="top:'
                                    + jsonObj.top
                                    + 'px; left:'
                                    + jsonObj.left
                                    + 'px;" href="'
                                    + jsonObj.link
                                    + '" target="_blank">'
                                    + contentStr
                                    + '</a>';
                                $overlay.append(markup);
                                $(this).parent().remove();
                              });

                      // Process lightbox link fields.
                      $('.views-field-field-vsx-lightbox-link-value .field-content,'
                              + '.views-field-field-vsx-lightbox-link-value .views-content-field-vsx-lightbox-link-value',
                          this)
                          .each(

                              function(i) {
                                var content = $(this).html();
                                if (content.length == 0)
                                  return;
                                var re = /(\{[^\}]*\})/;
                                var jsonStr = re.exec(content);
                                var contentStr = content.replace(re, "");
                                var jsonObj = eval("(" + jsonStr[0] + ")");
                                var markup = '<span id="views-slideshow-xtra-lightbox-link-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '" class="views-slideshow-xtra views-slideshow-xtra-lightbox-link '
                                    + 'views-slideshow-xtra-slide-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '" style="top:'
                                    + jsonObj.top
                                    + 'px; left:'
                                    + jsonObj.left
                                    + 'px;">'
                                    + contentStr + '</span>';
                                $overlay.append(markup);
                                $(this).parent().remove();
                              });

                      // Process lightbox title fields.
                      $('.views-field-field-vsx-lightbox-title-value .field-content,'
                              + '.views-field-field-vsx-lightbox-title-value .views-content-field-vsx-lightbox-title-value',
                          this)
                          .each(

                              function(i) {
                                var content = $(this).html();
                                if (content.length == 0)
                                  return;
                                var markup = '<div id="views-slideshow-xtra-lightbox-title-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '" class="views-slideshow-xtra-lightbox-element views-slideshow-xtra-lightbox-title '
                                    + 'views-slideshow-xtra-slide-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '">'
                                    + content + '</div>';
                                $lightBoxBody.append(markup);
                                $(this).parent().remove();
                              });

                      // Process lightbox text fields.
                      $('.views-field-field-vsx-lightbox-text-value .field-content,'
                              + '.views-field-field-vsx-lightbox-text-value .views-content-field-vsx-lightbox-text-value',
                          this)
                          .each(

                              function(i) {
                                var content = $(this).html();
                                if (content.length == 0)
                                  return;
                                var markup = '<div id="views-slideshow-xtra-lightbox-text-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '" class="views-slideshow-xtra-lightbox-element views-slideshow-xtra-lightbox-text '
                                    + 'views-slideshow-xtra-slide-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '">'
                                    + content + '</div>';
                                $lightBoxBody.append(markup);
                                $(this).parent().remove();
                              });

                      // Process lightbox video fields.
                      $('.views-field-field-vsx-lightbox-video-embed .field-content,'
                              + '.views-field-field-vsx-lightbox-video-embed .views-content-field-vsx-lightbox-video-embed',
                          this)
                          .each(

                              function(i) {
                                var content = $(this).html();
                                if (content.length == 0)
                                  return;
                                var markup = '<div id="views-slideshow-xtra-lightbox-video-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '" class="views-slideshow-xtra-lightbox-element views-slideshow-xtra-lightbox-video '
                                    + 'views-slideshow-xtra-slide-'
                                    + slideShowIndex
                                    + '-'
                                    + slideIndex
                                    + '">'
                                    + content + '</div>';
                                $lightBoxBody.append(markup);
                                $(this).parent().remove();
                              });

                    }); // End slide.
            // Pause animation on hover and mousemove. This is done to prevent
            // the situation where a user moves the mouse pointer towards a link
            // and the slide transitions before the user is able to click the
            // link.
            // TODO: make this value configurable
            var pageX = 0, pageY = 0, timeout;
            $overlay.mousemove(function(e) {
              if (pageX - e.pageX > 5 || pageY - e.pageY > 5) {
                $slideShow.cycle("pause");
                clearTimeout(timeout);
                timeout = setTimeout(function() {
                  $slideShow.cycle("resume");
                }, 2000);
              }
              pageX = e.pageX;
              pageY = e.pageY;
            });

            // Make lightbox links clickable.
            $('.views-slideshow-xtra-lightbox-link', $slideShow).click(

                function(e) {
                  // Pause slide show.
                  $slideShow.cycle("pause");
                  clearTimeout(timeout);
                  // Invoke lightbox black background.
                  $("#views-slideshow-xtra-lightbox-bg-" + slideShowIndex).css(
                      "opacity", 0).show().fadeTo(
                      "normal",
                      0.6,
                      function() {
                        // Slide down lightbox.
                        var left = ($(window).width() - $(
                            "#views-slideshow-xtra-lightbox-" + slideShowIndex)
                            .width()) / 2;
                        $("#views-slideshow-xtra-lightbox-" + slideShowIndex)
                            .css("left", left + "px").slideDown();
                      });
                });

            // Close lightbox when background is clicked.
            $(
                ".views-slideshow-xtra-lightbox-bg, .views-slideshow-xtra-lightbox-x")
                .click(

                    function(e) {
                      var $lightBoxBg = $('.views-slideshow-xtra-lightbox-bg',
                          $slideShow);
                      var $lightBox = $('.views-slideshow-xtra-lightbox',
                          $slideShow);

                      // $lightBoxBg.hide();
                      // $lightBox.hide();
                      // Slide up the lightbox and fade out the background.
                      $lightBox.slideUp("normal", function() {
                        $lightBoxBg.fadeOut();
                      });

                      // Destroy the video element so the video stops playing
                      // when the lightbox is closed (IE only).
                      var isIE = navigator.appName.indexOf("Microsoft") != -1;
                      if (isIE) {
                        $(".views-slideshow-xtra-lightbox-video", $lightBox)
                            .html(
                                '<div><span style="font-weight:bold; text-align:center;">'
                                    + 'Please press F5 to refresh slide show.</span></div>');
                      }

                      // Resume slide show.
                      $slideShow.cycle("resume");
                    });

            // Display links if only one slide ("before" does not run).
            // TODO This does not work - for now single slide slideshows are not
            // supported.
            /*
             * if ($(".views_slideshow_slide", $slideShow).length == 1) {
             * setTimeout(function() { alert($(".views-slideshow-xtra-slide-" +
             * $slideShow.data("slideShowIndex") + "-0").length);
             * 
             * $(".views-slideshow-xtra-slide-" +
             * $slideShow.data("slideShowIndex") + "-0").show(); }, 850); }
             */

          }); // End slideshow.
  // Utility function that grafts code from the second function
  // into the first - return string must be evaluated to work.

  function _vss_graft_functions(func1, func2) {
    func1 = func1.toString();
    func2 = func2.toString();
    return func1.substr(0, func1.length - 2)
        + func2.substring(func2.indexOf("{") + 1, func2.lastIndexOf("}")) + '}';
  }

};

