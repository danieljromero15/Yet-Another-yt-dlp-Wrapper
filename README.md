A project with the goal of simplifying yt-dlp downloads from YouTube

Goals:

* Download yt-dlp from GitHub if not found in repo or path
  * Option to update executable if old version? or provide warning
  * Do the same with ffmpeg
* Provide various features after inputting link, including:
  * Video preview, similar to Handbrake
  * List of all options, nicely formatted (-F)
  * Provide conversion options to standard formats if needed




* Current features:
  * YouTube API retrieval (Video name, uploader, duration, thumbnail)
  * Checks if yt-dlp is installed
  * Downloads video to executable directory. No options implemented yet.