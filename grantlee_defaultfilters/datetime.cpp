/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "datetime.h"

#include <QDateTime>

#include "util_p.h"

QVariant timeSince( QDateTime early, QDateTime late )
{
  Q_ASSERT( early.isValid() );
  Q_ASSERT( late.isValid() );

  int secsSince = early.secsTo( late );

  if ( secsSince < 0 )
    return SafeString( "0 minutes" );

  // TODO: i18n
  QStringList singularNames;
  singularNames << "year" << "month" << "week" << "day" << "hour" << "minute";
  QStringList pluralNames;
  pluralNames << "years" << "months" << "weeks" << "days" << "hours" << "minutes";

  QList<int> seconds;
  seconds << ( 60 * 60 * 24 * 365 ) // year
          << ( 60 * 60 * 24 * 30 ) // month
          << ( 60 * 60 * 24 * 7 ) // week
          << ( 60 * 60 * 24 ) // day
          << ( 60 * 60 ) // hour
          << ( 60 ); // minute

  int count = secsSince;
  int i = 0;
  while ( i < seconds.size() ) {
    count = ( int )( secsSince / seconds.at( i ) );
    ++i;
    if ( count != 0 )
      break;
  }
  QString firstChunk;

  if ( count != 1 )
    firstChunk.append( QString( "%1 %2" ).arg( count ).arg( pluralNames.at( i - 1 ) ) );
  else {
    firstChunk.append( QString( "%1 %2" ).arg( count ).arg( singularNames.at( i - 1 ) ) );
  }
  if ( seconds.size() > i ) {
    int count2 = ( secsSince - ( seconds.at( i - 1 ) * count ) ) / seconds.at( i );
    if ( count2 != 0 ) {
      if ( count2 > 1 )
        firstChunk.append( QString( ", %1 %2" ).arg( count2 ).arg( pluralNames.at( i ) ) );
      else
        firstChunk.append( QString( ", %1 %2" ).arg( count2 ).arg( singularNames.at( i ) ) );
    }
  }
  return firstChunk;
}

QVariant timeUntil( QDateTime dt, QDateTime now = QDateTime() )
{
  if ( !now.isValid() )
    now = QDateTime::currentDateTime();

  return timeSince( now, dt );
}


DateFilter::DateFilter()
{
}

QVariant DateFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  QDateTime d = QDateTime::fromString( Util::getSafeString( input ), "yyyy-MM-ddThh:mm:ss" );

  SafeString argString = Util::getSafeString( argument );

  if ( !argString.isEmpty() )
    return d.toString( argString );

  return d.toString( "MMM. d, yyyy" );
}

TimeFilter::TimeFilter()
{
}

QVariant TimeFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  SafeString argString = Util::getSafeString( argument );
  return QDateTime::fromString( Util::getSafeString( input ), "yyyy-MM-ddThh:mm:ss" ).toString( argString );
}


TimeSinceFilter::TimeSinceFilter()
{
}

QVariant TimeSinceFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  QDateTime late;
  SafeString argString = Util::getSafeString( argument );
  if ( argString.isEmpty() )
    late = QDateTime::currentDateTime();
  else
    late = QDateTime::fromString( argString, "yyyy-MM-ddThh:mm:ss" );

  QDateTime early = QDateTime::fromString( Util::getSafeString( input ), "yyyy-MM-ddThh:mm:ss" );
  return timeSince( early, late );
}


TimeUntilFilter::TimeUntilFilter()
{
}

QVariant TimeUntilFilter::doFilter( const QVariant& input, const QVariant &argument, bool autoescape ) const
{
  QDateTime early;
  SafeString argString = Util::getSafeString( argument );
  if ( argString.isEmpty() )
    early = QDateTime::currentDateTime();
  else
    early = QDateTime::fromString( argString, "yyyy-MM-ddThh:mm:ss" );

  QDateTime late = QDateTime::fromString( Util::getSafeString( input ), "yyyy-MM-ddThh:mm:ss" );

  return timeSince( early, late );
}
