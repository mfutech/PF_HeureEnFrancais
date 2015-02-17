DIR=File.dirname __FILE__
Dir.chdir DIR

task :build do |t| 
  %x[jekyll  b]
end

task :publish => [:build] do |t|
  Dir.chdir '_site'
  %x[git commit -a -m 'update']
  %x[git push origin gh-pages]
end
