README.txt
==========

This module provides the use of Batch API for aliasing content.
In case of large sites with huge magnitude of content this method would come in handy as the Batch API by design helps prevent timeout errors
The actual aliasing process is still done by the pathauto module's bulk alias generation functionalities and so this module depends on pathauto.
Though Batch API has the restriction of being terminated when the browser window running the batch process is closed it can be overcome by the Background batch module which is bundled along with the Background process module(https://drupal.org/project/background_process).

AUTHOR/MAINTAINER
==================

-viswanath gopalakrishnan <viswanathksg@yahoo.co.in>