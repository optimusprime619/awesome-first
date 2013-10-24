INTRODUCTION:
============

This module provides the use of Batch API for aliasing content & users.

In case of sites with large magnitude of content this method would 
come in handy as the Batch API by design helps prevent timeout errors.

The actual aliasing process is still done by the pathauto module's 
bulk alias generation functionalities and so this module depends on pathauto.

Though Batch API has the restriction of being terminated when the 
browser window running the batch process is closed it can be overcome 
by the Background batch module which is bundled along with 
the Background process module(https://drupal.org/project/background_process).

PERMISSIONS:
============

This module has two permissions namely 
'Configure batch aliasing settings' for access to batch settings form 
'Run batch aliasing of content' for access to running the batch aliasing.

RUN BATCH ALIASING:
===================

Configure the settings for Batch aliasing by choosing the 
necessary node types and if users need to be aliased.

If a content type is disabled it might either mean that 
there are no nodes of that type or 
it lacks its own node patterns which can be configured 
in the pathauto alias settings. 

The configuration form is available by navigating to 
Administer >> Configuration >> Development >> 
Configure Batch Alias Settings & Run Batch Alias.

Once the configuration is saved Click the 
'Run Batch Alias' to select from the confirmation form 
and run the batch aliasing. The process would alias all content types
and users if selected.


AUTHOR/MAINTAINER
==================

-viswanath gopalakrishnan <viswanathksg@yahoo.co.in>
https://drupal.org/user/881218