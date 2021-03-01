# automake does not accept any extension on the required files, but GitLab doesn't recognise them as a Org files without the .org extension
mv README.org README
mv NEWS.org NEWS

autoreconf --install || exit 1

mv README README.org
mv NEWS NEWS.org
