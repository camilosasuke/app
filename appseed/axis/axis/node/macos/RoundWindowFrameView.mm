//
//  RoundWindowFrameView.m
//  RoundWindow
//
//  Created by Matt Gallagher on 12/12/08.
//  Copyright 2008 Matt Gallagher. All rights reserved.
//
//  Permission is given to use this source code file without charge in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.
//
#import "macos_mm.h"


@implementation RoundWindowFrameView

//
// resizeRect
//
// Returns the bounds of the resize box.
//
- (NSRect)resizeRect
{
   
	const CGFloat resizeBoxSize = 16.0;
   
	const CGFloat contentViewPadding = 5.5;
	
	NSRect contentViewRect = [[self window] contentRectForFrameRect:[[self window] frame]];
   
	NSRect resizeRect = NSMakeRect(
		NSMaxX(contentViewRect) + contentViewPadding,
		NSMinY(contentViewRect) - resizeBoxSize - contentViewPadding,
		resizeBoxSize,
		resizeBoxSize);
	
	return resizeRect;
   
}

- (void)mouseUp:(NSEvent *)event
{
   
   round_window * p = m_roundwindow->m_pwindow;
   
   if(p == NULL)
      return;
   
   NSPoint point = [[self window] convertBaseToScreen:[event locationInWindow]];
   
   NSRect e = [[NSScreen mainScreen] frame];
   
   int H = (int) e.size.height;
   
   int x = point.x;
   
   int y = H - point.y;
   
   p->round_window_mouse_up(x, y);
   
   return;
   
}


- (void)mouseMoved:(NSEvent *)event
{
   
   round_window * p = m_roundwindow->m_pwindow;
   
   if(p == NULL)
      return;
   
   NSPoint point = [[self window] convertBaseToScreen:[event locationInWindow]];
   
   NSRect e = [[NSScreen mainScreen] frame];
   
   int H = (int) e.size.height;
   
   int x = point.x;
   
   int y = H - point.y;
   
   p->round_window_mouse_moved(x, y);
   
   return;
   
}


- (void)mouseDragged:(NSEvent *)event
{
   
   round_window * p = m_roundwindow->m_pwindow;
   
   if(p == NULL)
      return;
   
   NSPoint point = [[self window] convertBaseToScreen:[event locationInWindow]];
   
   NSRect e = [[NSScreen mainScreen] frame];
   
   int H = (int) e.size.height;
   
   int x = point.x;
   
   int y = H - point.y;
   
   p->round_window_mouse_dragged(x, y);
   
   return;
   
   
}


//
// mouseDown:
//
// Handles mouse clicks in our frame. Two actions:
//	- click in the resize box should resize the window
//	- click anywhere else will drag the window.
//
- (void)mouseDown:(NSEvent *)event
{
   
   round_window * p = m_roundwindow->m_pwindow;
   
   if(p == NULL)
      return;
   
   NSPoint point = [[self window] convertBaseToScreen:[event locationInWindow]];
   
   NSRect e = [[NSScreen mainScreen] frame];
   
   int H = (int) e.size.height;
   
   int x = point.x;
   
   int y = H - point.y;
   
   p->round_window_mouse_down(x, y);
   
   return;
   
   /*
   
	NSPoint pointInView = [self convertPoint:[event locationInWindow] fromView:nil];
	
	BOOL resize = NO;
	if (NSPointInRect(pointInView, [self resizeRect]))
	{
		resize = YES;
	}
	
	NSWindow *window = [self window];
	NSPoint originalMouseLocation = [window convertBaseToScreen:[event locationInWindow]];
	NSRect originalFrame = [window frame];
	
    while (YES)
	{
		//
		// Lock focus and take all the dragged and mouse up events until we
		// receive a mouse up.
		//
        NSEvent *newEvent = [window
			nextEventMatchingMask:(NSLeftMouseDraggedMask | NSLeftMouseUpMask)];
		
        if ([newEvent type] == NSLeftMouseUp)
		{
			break;
		}
		
		//
		// Work out how much the mouse has moved
		//
		NSPoint newMouseLocation = [window convertBaseToScreen:[newEvent locationInWindow]];
		NSPoint delta = NSMakePoint(
			newMouseLocation.x - originalMouseLocation.x,
			newMouseLocation.y - originalMouseLocation.y);
		
		NSRect newFrame = originalFrame;
		
		if (!resize)
		{
			//
			// Alter the frame for a drag
			//
			newFrame.origin.x += delta.x;
			newFrame.origin.y += delta.y;
		}
		else
		{
			//
			// Alter the frame for a resize
			//
			newFrame.size.width += delta.x;
			newFrame.size.height -= delta.y;
			newFrame.origin.y += delta.y;
			
			//
			// Constrain to the window's min and max size
			//
			NSRect newContentRect = [window contentRectForFrameRect:newFrame];
			NSSize maxSize = [window maxSize];
			NSSize minSize = [window minSize];
			if (newContentRect.size.width > maxSize.width)
			{
				newFrame.size.width -= newContentRect.size.width - maxSize.width;
			}
			else if (newContentRect.size.width < minSize.width)
			{
				newFrame.size.width += minSize.width - newContentRect.size.width;
			}
			if (newContentRect.size.height > maxSize.height)
			{
				newFrame.size.height -= newContentRect.size.height - maxSize.height;
				newFrame.origin.y += newContentRect.size.height - maxSize.height;
			}
			else if (newContentRect.size.height < minSize.height)
			{
				newFrame.size.height += minSize.height - newContentRect.size.height;
				newFrame.origin.y -= minSize.height - newContentRect.size.height;
			}
		}
		
		[window setFrame:newFrame display:YES animate:NO];
	}
   
   */
}

- (BOOL) isFlipped
{
   return YES;
}


#define WINDOW_FRAME_PADDING 32

//
// drawRect:
//
// Draws the frame of the window.
//
- (void)drawRect:(NSRect)rect
{
   
   /*
   
   	[[NSColor clearColor] set];
	NSRectFill(rect);


	NSBezierPath * rectPath = [NSBezierPath bezierPathWithRect : [self bounds]];
	
	NSGradient * gradient = [[NSGradient alloc] initWithColorsAndLocations : [NSColor whiteColor], (CGFloat) 0.0, [NSColor lightGrayColor], (CGFloat)1.0, nil];

   [gradient drawInBezierPath : rectPath angle:90];

	[[NSColor whiteColor] set];
	[rectPath stroke];
	
   NSRect resizeRect = [self resizeRect];
	NSBezierPath *resizePath = [NSBezierPath bezierPathWithRect:resizeRect];

	[[NSColor lightGrayColor] set];
	[resizePath fill];

	[[NSColor darkGrayColor] set];
	[resizePath stroke];
	

   [[NSColor blackColor] set];
	NSString *windowTitle = [[self window] title];
	NSRect titleRect = [self bounds];
	titleRect.origin.y = titleRect.size.height - (WINDOW_FRAME_PADDING - 7);
	titleRect.size.height = (WINDOW_FRAME_PADDING - 7);
	NSMutableParagraphStyle *paragraphStyle =
		[[NSMutableParagraphStyle alloc] init];
	[paragraphStyle setAlignment:NSCenterTextAlignment];
	[windowTitle
		drawWithRect:titleRect
		options:0
		attributes:[NSDictionary
			dictionaryWithObjectsAndKeys:
				paragraphStyle, NSParagraphStyleAttributeName,
				[NSFont systemFontOfSize:14], NSFontAttributeName,
			nil]];
 
   
   return;*/
  
   round_window * p = m_roundwindow->m_pwindow;
   
   if(p == NULL)
      return;
   
   //   [m_roundwindow disableScreenUpdatesUntilFlush];
   
//   [m_roundwindow disableFlushWindow];
   
   CGContextRef cgc = (CGContextRef) [[NSGraphicsContext currentContext] graphicsPort];
   
   p->round_window_draw(cgc);
   
  // [m_roundwindow enableFlushWindow];
   
//   [m_roundwindow flushWindow];
   
}


- (BOOL)acceptsFirstResponder
{
   
   return TRUE;
   
}


- (BOOL) acceptsFirstMouse:(NSEvent *)theEvent
{

   
/*   if(theEvent != NULL)
   {
   
      [self mouseDown: theEvent];
      
   }*/
   
   return YES;
   
}


- (BOOL)mouseDownCanMoveWindow
{

   return NO;

}


// from RoundWindow.mm

/*- (BOOL) acceptsFirstResponder
{
   if(m_bNoActivate)
      return NO;
   else
      return YES;
}*/

- (BOOL) becomeFirstResponder
{
//   if(m_bNoActivate)
  //    return NO;
  // else
   {
      
      //      m_pwindow->round_window_on_become_first_responder();
      
      return YES;
      
   }
}

- (BOOL) resignFirstResponder
{
   
   //if(m_bNoActivate)
     // return YES;
   //else
      return YES;
   
}


- (void)keyDown:(NSEvent *)event
{

   unsigned int uiKeyCode = event_key_code(event);
   
   round_window * p = m_roundwindow->m_pwindow;
   
   if(p == NULL)
      return;
   
   if(p->round_window_key_down(uiKeyCode))
      return;
   
   [super keyDown:event];
   
}


// end from RoundWindow.mm
- (void)keyUp:(NSEvent *)event
{
   
   unsigned int uiKeyCode = event_key_code(event);
   
   round_window * p = m_roundwindow->m_pwindow;
   
   if(p == NULL)
      return;
   
   if(p->round_window_key_up(uiKeyCode))
      return;
   
   [super keyUp:event];
   
}

- (void)flagsChanged:(NSEvent *)event
{

   round_window * p = m_roundwindow->m_pwindow;
   
   if(p == NULL)
      return;
   
   if([event modifierFlags] & NSShiftKeyMask)
   {
      if(!m_bShift)
      {
         m_bShift = true;
         if(p->round_window_key_down(::user::key_shift))
            return;
      }
   }
   else
   {
      if(m_bShift)
      {
         m_bShift = false;
         if(p->round_window_key_up(::user::key_shift))
            return;
      }
   }

   if([event modifierFlags] & NSControlKeyMask)
   {
      if(!m_bControl)
      {
         m_bControl = true;
         if(p->round_window_key_down(::user::key_control))
            return;
      }
   }
   else
   {
      if(m_bControl)
      {
         m_bControl = false;
         if(p->round_window_key_up(::user::key_control))
            return;
      }
   }
 
   if([event modifierFlags] & NSAlternateKeyMask)
   {
      if(!m_bAlt)
      {
         m_bAlt = true;
         if(p->round_window_key_down(::user::key_alt))
            return;
      }
   }
   else
   {
      if(m_bAlt)
      {
         m_bAlt = false;
         if(p->round_window_key_up(::user::key_alt))
            return;
      }
   }

   [super flagsChanged:event];
}

@end


unsigned int event_key_code(NSEvent * event)
{
   
   if([event modifierFlags] & NSNumericPadKeyMask) // arrow keys have this mask
   {
      
      NSString * arrow = [event charactersIgnoringModifiers];
      
      unichar key = 0;
      
      if([arrow length] == 0)
         return ::user::key_none;            // reject dead keys
      
      if([arrow length] == 1)
      {
         
         key = [arrow characterAtIndex:0];
         
         if(key == NSLeftArrowFunctionKey)
         {
            
            return 1002; //::user::key_left;
            
         }
         
         if(key == NSRightArrowFunctionKey)
         {
            
            return 1004; //::user::key_right;
            
         }
         
         if(key == NSUpArrowFunctionKey)
         {
            
            return 1003; // ::user::key_up;
            
         }
         
         if(key == NSDownArrowFunctionKey)
         {
            
            return 1005; // ::user::key_down;
            
         }
         
      }
      
   }
   else
   {
      
      unsigned short ush = [event keyCode];
      
      return ush;
      
   }
   
   return ::user::key_none;;
   
   
}
