<? include('httpProtocol.php'); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"

        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<link rel="stylesheet" href="./css/slideshow.css" type="text/css" media="screen" title="no title" />
<head>
<title><?php print $head_title; ?></title>
<script type="text/javascript">
<!--//--><![CDATA[//><!--
var ASUHeader = ASUHeader || {};
ASUHeader.user_signedin = <?=$asu_sso_signedin?>;
ASUHeader.signin_url = "<?=$asu_sso_signinurl?>";
ASUHeader.signout_url = "<?=$asu_sso_signouturl?>";
//--><!]]>
</script>
<?php include($asu_header_basepath.'/'.$asu_header_version.'/heads/'.$asu_header_template.'.shtml'); ?>

<?php if ($asu_ga_alternate): ?>
<script type="text/javascript">
<!--//--><![CDATA[//><!--
_gaq.push(['_setAccount', '<?php echo $asu_ga_alternate; ?>'], ['_setDomainName', '.asu.edu'], ['_trackPageview']);
//--><!]]>
</script>
<?php endif; ?>

<?php print $head; ?>
<?php print $styles; ?>
<?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>">

	<!-- START skip links -->
	<ul class="skip">
	<li><a href="#main">Skip to content</a></li>
	<li><a href="#sidebar-left">Skip to navigation</a></li>
		<!-- Including a site map is especially helpful to users dependent on assistive technology.
		To create a quick sitemap, download the Sitemap Module ( http://drupal.org/project/site_map ); install the module in your folder /sites/all/modules (create folder if necessary); and enable it in http://YourSite.asu.edu/admin/build/modules. Then uncomment the link below. That's it! -->
	<!-- <li><a href="/sitemap">Skip to site map page</a></li> -->
	</ul>
	<!-- /.skip links -->

<div class="asu_set_fixed_width">


<!-- START #asu_container -->  	
<div id="asu_container">
  <?php include($asu_header_basepath.'/'.$asu_header_version.'/headers/'.$asu_header_template.'.shtml'); ?>
  <?php include($physics_header); ?>
  <?php include($pirt_header); ?>
  <?php if ($navbar): ?>
        <div id="navbar"><div id="navbar-inner" class="clear-block region region-navbar">
			<?php print $navbar; ?>
        </div></div> <!-- /#navbar-inner, /#navbar -->
  <?php endif; ?>
  
  <?php if ($header): ?>
	<div id="header-blocks" class="region region-header">
	  <?php print $header; ?>
	</div> <!-- /#header-blocks -->
  <?php endif; ?>
      
   <!-- START main -->
   <div id="main"><div id="main-inner" class="clear-block<?php if ($navbar) { print ' with-navbar'; } ?>">

    <div id="content"><div id="content-inner">
    
	<?php if ($breadcrumb or $title or $tabs or $help or $messages): ?>
	  <div id="content-header">
		<?php print $breadcrumb; ?>
		<?php if ($title): ?><h1 class="title"><?php print $title; ?></h1><?php endif; ?>
                <?php if ($mission): ?><div id="mission"><?php print $mission; ?></div><?php endif; ?>
		<?php if ($content_top): ?><div id="content-top" class="region region-content_top">
			<?php print $content_top; ?></div> <!-- /#content-top -->
		<?php endif; ?>
		<?php if ($tabs): ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>
		<?php print $help; ?>
		<?php print $messages; ?>
	  </div> <!-- /#content-header -->
 	<?php endif; ?>

	  <div id="content-area">
	    <?php print $content; ?>
	  </div>
	  <?php if ($feed_icons): ?><div class="feed-icons"><?php print $feed_icons; ?></div><?php endif; ?>

	  <?php if ($content_bottom): ?><div id="content-bottom" class="region region-content_bottom">
		<?php print $content_bottom; ?>
	  </div> <!-- /#content-bottom --><?php endif; ?>

    </div></div> <!-- /#content-inner, /#content -->

      <?php if ($left || $logo || $site_name || $site_slogan): ?> 
        <div id="sidebar-left"><div id="sidebar-left-inner" class="region region-left">
        	<!-- START name-and-slogan -->

			<?php if ($logo || $site_name || $site_slogan): ?>
			<div id="name-and-slogan">
			
				<?php if ($logo): ?>
				<a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>">
				<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="site-logo" />
				</a>
				<?php endif; ?>
			
				<?php if ($site_name): ?>
				<h1 id='site-name'>
				<a href="<?php print $base_path ?>" title="<?php print t('Home'); ?>">
				<?php print $site_name; ?>
				</a>
				</h1>
				<?php endif; ?>
			
				<?php if ($site_slogan): ?>
				<div id='site-slogan'>
				<?php print $site_slogan; ?>
				</div>
				<?php endif; ?>
			</div> 
			<?php endif; ?>
			<!-- /#name and slogan -->


        <?php print $left; ?>
        </div></div> <!-- /#sidebar-left-inner, /#sidebar-left -->
      <?php endif; ?>

      <?php if ($right): ?>
        <div id="sidebar-right"><div id="sidebar-right-inner" class="region region-right">
          <?php print $right; ?>
        </div></div> <!-- /#sidebar-right-inner, /#sidebar-right -->
      <?php endif; ?>

    </div></div> <!-- /#main-inner, /#main -->
	
	<?php include($asu_header_basepath.'/'.$asu_header_version.'/includes/footer'.$asu_footer_color.'.shtml'); ?>
	
    <?php if ($footer || $footer_message): ?>
      <div id="asu_footer_contact_info"><div id="footer-inner" class="region region-footer">

        <?php if ($footer_message): ?>
          <p><?php print $footer_message; ?></p>
        <?php endif; ?>

          <?php print $footer; ?>

	  </div></div><!-- end footer contact info -->
	<?php endif; ?>

</div><!-- /#asu-container -->

</div><!-- /.asu_set_fixed_width -->

  <?php if ($closure_region): ?>
    <div id="closure-blocks" class="region region-closure"><?php print $closure_region; ?></div>
  <?php endif; ?>

<?php print $closure; ?>

<!--[lte IE 6]>
<script type="text/javascript" src="<?php echo base_path() . path_to_theme() . "/jquery.pngFix-1.1.min.js"; ?>"></script>
<![endif]-->
<?php
 global $base_root;
 $isDev = strpos($base_root, "-dev");
 if(!$isDev)
 {
   include('/afs/asu.edu/www/asuthemes/latest/includes/html/google_analytics.shtml');
?>
<!-- Begin Google Analytics -->
<script type="text/javascript">
var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
</script>
<script type="text/javascript">
var pageTracker = _gat._getTracker("UA-561964-1");
pageTracker._initData();
pageTracker._trackPageview();
</script>
<!-- End Google Analytics -->
<?php } ?>
<script type="text/javascript">
   jQuery("h2[class='pane-title']").addClass("gradient-backing");
</script>

</body>
</html>

