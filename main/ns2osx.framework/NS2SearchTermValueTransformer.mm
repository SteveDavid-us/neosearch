//
//  NS2SearchTermValueTransformer.mm
//  ns2osx.framework
//
//  Created by Michael Pigg on 10/20/09.
//  Transfomer to map nil values to empty string for search terms where no value is selected.
//

#import "NS2SearchTermValueTransformer.h"


@implementation NS2SearchTermValueTransformer
+ (Class)transformedValueClass {
	return [NSString class];
}

+ (BOOL)allowsReverseTransformation {
	return YES;
}

- (id)transformedValue:(id)value {
	if (value == @"") {
		return nil;
	} else {
		return value;
	}
}

- (id)reverseTransformedValue:(id)value {
	if (value == nil) {
		return @"";
	} else {
		return value;
	}
}

@end
